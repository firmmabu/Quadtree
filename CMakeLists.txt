cmake_minimum_required(VERSION 3.6.0)

project(quadtree LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER "g++")

set(SRCS
    ./src/main.cpp

    ./header/quadtree.h
    ./src/quadtree.cpp

    ./header/common.h
    ./src/common.cpp
)

add_executable(main ${SRCS})

set(CMAKE_BUILD_TYPE "Debug")
