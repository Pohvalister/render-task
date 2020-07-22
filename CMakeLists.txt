cmake_minimum_required(VERSION 3.16)
project(RenderTask)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(googletest)

include_directories(googletest/googletest/include)
add_executable(RenderTask main.cpp int_to_bin.h list_serialize.h list_serialize.cpp)
target_link_libraries(RenderTask gtest gtest_main)