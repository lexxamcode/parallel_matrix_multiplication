cmake_minimum_required(VERSION 3.23)
project(mm_simple)

set(CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(create
        create.cpp)
add_executable(load load.cpp)
target_link_libraries(load ${MPI_C_LIBRARIES})