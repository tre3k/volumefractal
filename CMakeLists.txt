cmake_minimum_required(VERSION 3.18)

project(fft3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -O3 -march=native")

set(INCLUDE_PATH "include/")
include_directories(${INCLUDE_PATH})

add_library(data3d STATIC fft3ddata.cpp ${INCLUDE_PATH}fft3ddata.h)

add_executable(fft3d
                main.cpp
                fastfouriertransform3d.cpp ${INCLUDE_PATH}fastfouriertransform3d.h
                options.cpp ${INCLUDE_PATH}options.h)

target_link_libraries(fft3d data3d)
