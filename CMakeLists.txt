cmake_minimum_required(VERSION 3.23)
project(mm_simple)

set(CMAKE_CXX_STANDARD 14)

add_executable(create
        create.cpp)
add_executable(load load.cpp)