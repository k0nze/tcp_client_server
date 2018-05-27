cmake_minimum_required(VERSION 2.8)
project(c_project_template)

set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR})
set(EXCUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main ${PROJECT_SOURCE_DIR}/src/main.c)
