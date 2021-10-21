cmake_minimum_required(VERSION 3.18)

project(generate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -O3 -march=native")

set(INCLUDE_PATH "../fft3d")
include_directories(${INCLUDE_PATH})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../bin)


set(LIB_PATH ${CMAKE_SOURCE_DIR}/../lib)
link_directories(${LIB_PATH})

add_executable(
  generate
  main.cpp
  primitives.cpp primitives.h
  fractals.cpp fractals.h
  )


target_link_libraries(generate data3d)
