cmake_minimum_required(VERSION 3.23)
project(mmultiplication)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(mmultiplication main.cpp functions.h)
add_executable(proverka proverka.cpp functions.h)
