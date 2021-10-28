cmake_minimum_required(VERSION 3.16)

project(fft3d LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_library(data3d STATIC fft3ddata.cpp fft3ddata.h)

add_executable(fft3d
               main.cpp
               fastfouriertransform3d.cpp
	       ${INCLUDE_PATH}fastfouriertransform3d.h
               )

target_link_libraries(fft3d data3d)
