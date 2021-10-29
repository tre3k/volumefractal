cmake_minimum_required(VERSION 3.16)

project(average LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(INCLUDE_PATH ${CMAKE_SOURCE_DIR}/fft3d)
set(LIB_PATH ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

include_directories(${INCLUDE_PATH})
link_directories(${LIB_PATH})

add_library(aaverage STATIC average.cpp average.h)

add_executable(
  average
  main.cpp
  )


target_link_libraries(average
  data3d
  aaverage
  )
