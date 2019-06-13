cmake_minimum_required(VERSION 3.14)
project(ParallelHGA)

set(CMAKE_CXX_STANDARD 14)

add_executable(ParallelHGA main.cpp PGenetic.cpp PGenetic.h PHashGenetic.cpp PHashGenetic.h Graph.cpp Graph.h)