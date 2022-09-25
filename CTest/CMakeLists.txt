cmake_minimum_required(VERSION 3.23)
project(CTest)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(algorithm)
include_directories(homework)
include_directories(others)

add_executable(CTest
        algorithm/bubbleSort.c)
