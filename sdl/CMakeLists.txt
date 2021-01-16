cmake_minimum_required(VERSION 3.5)

project(sdl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_library(${PROJECT_NAME} sdl.cpp)

target_link_libraries(${PROJECT_NAME} -D_REENTRANT -pthread SDL2::SDL2 -lSDL2_ttf)
