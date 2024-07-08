cmake_minimum_required(VERSION 3.16)

project(simple-otel VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

include(cmake/common.cmake)

set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
set(PROTO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/proto")
file(MAKE_DIRECTORY "${GENERATED_DIR}")
# 按照基础proto 模型proto 和 服务proto 的顺序先后声明再合并 不然要出现找不到依赖的情况
file(GLOB_RECURSE PROTO_FILES_COMMON ${PROTO_PATH}/common/*.proto)
file(GLOB_RECURSE PROTO_FILES_MODEL ${PROTO_PATH}/model/*.proto)
file(GLOB_RECURSE PROTO_FILE_SERVICE ${PROTO_PATH}/collector/*.proto)
# 把 PROTO_FILES_COMMON, PROTO_FILES_MODEL, PROTO_FILE_SERVICE 合并到一起
set(PROTO_FILES ${PROTO_FILES_COMMON} ${PROTO_FILES_MODEL} ${PROTO_FILE_SERVICE})
set(PROTO_LIBS "")

# 我尝试写成一个function然后调用三遍 但这样生成不了动态链接库
foreach(PROTO_FILE ${PROTO_FILES})
    # 打印一下正在处理的文件
    message(STATUS "Processing proto file: ${PROTO_FILE}")
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
    get_filename_component(PARENT_DIR_NAME ${PROTO_FILE} DIRECTORY)
    get_filename_component(PARENT_DIR_NAME_WE ${PARENT_DIR_NAME} NAME)
    set(PROTO_SRC "${GENERATED_DIR}/${PARENT_DIR_NAME_WE}/${PROTO_NAME}.pb.cc")
    set(PROTO_HEADER "${GENERATED_DIR}/${PARENT_DIR_NAME_WE}/${PROTO_NAME}.pb.h")
    set(GRPC_SRC "${GENERATED_DIR}/${PARENT_DIR_NAME_WE}/${PROTO_NAME}.grpc.pb.cc")
    set(GRPC_HEADER "${GENERATED_DIR}/${PARENT_DIR_NAME_WE}/${PROTO_NAME}.grpc.pb.h")
    add_custom_command(
            OUTPUT "${PROTO_SRC}" "${PROTO_HEADER}" "${GRPC_SRC}" "${GRPC_HEADER}"
            COMMAND ${_PROTOBUF_PROTOC}
            ARGS --grpc_out "${GENERATED_DIR}"
            --cpp_out "${GENERATED_DIR}"
            -I "${PROTO_PATH}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${PROTO_FILE}"
            DEPENDS "${PROTO_FILE}"
    )
    # 生成相应的C++库
    add_library(${PROTO_NAME}_grpc_proto
            ${PROTO_SRC}
            ${PROTO_HEADER}
            ${GRPC_SRC}
            ${GRPC_HEADER}
    )
    # 链接Protobuf和gRPC库
    target_link_libraries(${PROTO_NAME}_grpc_proto
            ${_REFLECTION}
            ${_GRPC_GRPCPP}
            ${_PROTOBUF_LIBPROTOBUF}
    )
    list(APPEND PROTO_LIBS $<TARGET_OBJECTS:${PROTO_NAME}_grpc_proto>)
endforeach()

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(fmt)

include_directories("${GENERATED_DIR}")
include_directories(src/trace src/utils src/protocl src/log src/common src/metric)

foreach(SRC trace utils protocol log trace/exporter trace/processor trace/sampler log/processor log/exporter common metric metric/exporter)
   aux_source_directory(src/${SRC} SRC_LIST) 
endforeach()

foreach(_target client server main)
    add_executable(${_target} "src/${_target}.cpp" ${SRC_LIST}) 
    target_link_libraries(${_target} Boost::system Boost::filesystem fmt::fmt ${PROTO_LIBS} ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
endforeach()

foreach(_target grpc_span_exporter_test logger_test period_task_test metric_collector_test)
    add_executable(${_target} "test/${_target}.cpp" ${SRC_LIST}) 
    target_link_libraries(${_target} Boost::system Boost::filesystem fmt::fmt ${PROTO_LIBS} ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
endforeach(_target )
