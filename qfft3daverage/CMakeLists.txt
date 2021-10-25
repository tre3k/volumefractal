cmake_minimum_required(VERSION 3.18)

set(PROGRAM_NAME qfft3daverage)
project(${PROGRAM_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(INCLUDE_PATH ${CMAKE_SOURCE_DIR}/fft3d)
set(LIB_PATH ${CMAKE_SOURCE_DIR}/lib)

include_directories(${INCLUDE_PATH})
link_directories(${LIB_PATH})

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS PrintSupport REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)


add_executable(${PROGRAM_NAME}
  main.cpp
  mainwindow.cpp mainwindow.h
  processing.cpp processing.h
  qcustomplot.cpp qcustomplot.h
  iqcustomplot.cpp iqcustomplot.h
  )

target_link_libraries(${PROGRAM_NAME}
  data3d
  Qt5::Core
  Qt5::PrintSupport
  Qt5::Widgets
  )
