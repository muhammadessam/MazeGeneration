cmake_minimum_required(VERSION 3.8)
project(MazeGeneration)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)
link_libraries(cs m)
add_executable(MazeGeneration ${SOURCE_FILES})