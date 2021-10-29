cmake_minimum_required(VERSION 3.16)

project(generate LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(INCLUDE_PATH
  ${CMAKE_SOURCE_DIR}/fft3d
  ${CMAKE_SOURCE_DIR}/average
  )
set(LIB_PATH ${CMAKE_SOURCE_DIR}/lib)

include_directories(${INCLUDE_PATH})
link_directories(${LIB_PATH})

add_executable(
  generate
  main.cpp
  primitives.cpp primitives.h
  fractals.cpp fractals.h
  )

target_link_libraries(generate
  data3d
  aaverage
  )
