cmake_minimum_required(VERSION 3.5)

project(config_reader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_subdirectory(test)

add_library(${PROJECT_NAME} config_reader.cpp)

