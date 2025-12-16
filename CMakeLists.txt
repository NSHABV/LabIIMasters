cmake_minimum_required(VERSION 3.31)
project(Laboratory2AlgMaster)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(google_tests/lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

enable_testing()

add_executable(Laboratory2AlgMaster main.cpp
        array.cpp
        google_tests/test.cpp)
target_link_libraries(Laboratory2AlgMaster gtest gtest_main)
