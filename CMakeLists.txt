cmake_minimum_required(VERSION 3.5)
project(fd)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3")

add_subdirectory(table)
add_subdirectory(dfd)
add_subdirectory(tane)

add_executable(fd main.cpp util.h)

target_link_libraries(fd table dfd tane)

find_package(Threads REQUIRED)
target_link_libraries(fd Threads::Threads)