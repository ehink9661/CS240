cmake_minimum_required(VERSION 3.20)
project(KernelSim)

set(CMAKE_CXX_STANDARD 14)

add_executable(KernelSim main.cpp ksim.h ksim.cpp)
