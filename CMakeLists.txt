cmake_minimum_required(VERSION 3.16)

project(simple-otel VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

include_directories(src/trace src/utils)

add_executable(simple-otel src/main.cpp)

# target_link_libraries(simple-otel trace)