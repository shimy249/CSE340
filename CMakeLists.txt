cmake_minimum_required(VERSION 3.3)
project(CSE340Project1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES lexer.c lexer.h project3.cpp)
add_executable(CSE340Project1 ${SOURCE_FILES})