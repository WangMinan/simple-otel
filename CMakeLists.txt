cmake_minimum_required(VERSION 3.16)

project(simple-otel VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

include_directories(src/trace src/utils src/protocl)
find_package(Boost REQUIRED COMPONENTS system filesystem)

aux_source_directory(src/trace SRC_LIST)
aux_source_directory(src/utils SRC_LIST)
aux_source_directory(src/protocol SRC_LIST)
add_executable(simple-otel src/main.cpp ${SRC_LIST})
add_executable(server src/server.cpp ${SRC_LIST})
add_executable(client src/client.cpp ${SRC_LIST})
target_link_libraries(simple-otel Boost::system Boost::filesystem)
target_link_libraries(server Boost::system Boost::filesystem)
target_link_libraries(client Boost::system Boost::filesystem)

# target_link_libraries(simple-otel trace)