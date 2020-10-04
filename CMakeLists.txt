cmake_minimum_required(VERSION 3.18)

project(fft3d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(fft3d main.cpp fastfouriertransform3d.cpp fastfouriertransform3d.h)
