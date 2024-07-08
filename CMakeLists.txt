cmake_minimum_required(VERSION 3.16)

project(simple-otel VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

# # 设置 gRPC 安装路径
# set(CMAKE_PREFIX_PATH "/root/.local" ${CMAKE_PREFIX_PATH})

# # 查找 gRPC 和 Protocol Buffers 包
# find_package(Protobuf REQUIRED)
# find_package(gRPC REQUIRED)

# include(cmake/common.cmake)

# set(CMAKE_BINARY_DIR "/root/simple-otel/build")
# set(CMAKE_CURRENT_SOURCE_DIR "/root/simple-otel")

# set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
# set(PROTO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/proto")
# file(MAKE_DIRECTORY "${GENERATED_DIR}")
# file(GLOB_RECURSE PROTO_FILES ${PROTO_PATH}/*.proto)
# set(PROTO_LIBS "")
# foreach(PROTO_FILE ${PROTO_FILES})
#     get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE) 
#     get_filename_component(PARENT_DIR_NAME ${PROTO_FILE} DIRECTORY)
#     get_filename_component(PARENT_DIR_NAME_WE ${PARENT_DIR_NAME} NAME)
#     set(PROTO_SRC "${GENERATED_DIR}/${PARENT_DIR_NAME_WE}/${PROTO_NAME}.pb.cc")
#     set(PROTO_HEADER "${GENERATED_DIR}/${PARENT_DIR_NAME_WE}/${PROTO_NAME}.pb.h")
#     set(GRPC_SRC "${GENERATED_DIR}/${PARENT_DIR_NAME_WE}/${PROTO_NAME}.grpc.pb.cc")
#     set(GRPC_HEADER "${GENERATED_DIR}/${PARENT_DIR_NAME_WE}/${PROTO_NAME}.grpc.pb.h")
#     add_custom_command(
#         OUTPUT "${PROTO_SRC}" "${PROTO_HEADER}" "${GRPC_SRC}" "${GRPC_HEADER}"
#         COMMAND ${_PROTOBUF_PROTOC}
#         ARGS --grpc_out "${GENERATED_DIR}"
#             --cpp_out "${GENERATED_DIR}"
#             -I "${PROTO_PATH}"
#             --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
#             "${PROTO_FILE}"
#         DEPENDS "${PROTO_FILE}"
#     )
#     # 生成相应的C++库
#     add_library(${PROTO_NAME}_grpc_proto
#         ${PROTO_SRC}
#         ${PROTO_HEADER}
#         ${GRPC_SRC}
#         ${GRPC_HEADER}
#     )
#     # 链接Protobuf和gRPC库
#     target_link_libraries(${PROTO_NAME}_grpc_proto
#         ${_REFLECTION}
#         ${_GRPC_GRPCPP}
#         ${_PROTOBUF_LIBPROTOBUF} 
#     )
#     list(APPEND PROTO_LIBS $<TARGET_OBJECTS:${PROTO_NAME}_grpc_proto>)
# endforeach()

# include_directories("${GENERATED_DIR}")
include_directories(src/trace src/utils src/protocl src/log src/common src/metric)

foreach(SRC trace utils protocol log trace/exporter trace/processor trace/sampler log/processor log/exporter common metric metric/exporter)
   aux_source_directory(src/${SRC} SRC_LIST) 
endforeach()

foreach(_target client server main tcp_exporter_server)
    add_executable(${_target} "src/${_target}.cpp" ${SRC_LIST}) 
    target_link_libraries(${_target} -lpthread)
endforeach()

# foreach(_target grpc_client grpc_server)
#     add_executable(${_target} "src/${_target}.cpp" ${SRC_LIST}) 
#     target_link_libraries(${_target} ${PROTO_LIBS} ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
# endforeach()

foreach(_target span_exporter_test logger_test period_task_test metric_collector_test)
    add_executable(${_target} "test/${_target}.cpp" ${SRC_LIST}) 
    target_link_libraries(${_target} ${PROTO_LIBS} ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF} -lpthread)
endforeach()
