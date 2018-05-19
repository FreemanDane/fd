cmake_minimum_required(VERSION 3.10)
project(fd)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(table)

add_executable(fd main.cpp)

target_link_libraries(fd table)