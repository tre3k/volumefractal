cmake_minimum_required(VERSION 3.16)

project(volumefractal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -O3 -march=native")

add_subdirectory(fft3d)
add_subdirectory(generate)
add_subdirectory(average)
add_subdirectory(qviewer)
