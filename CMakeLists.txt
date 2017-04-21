cmake_minimum_required(VERSION 3.6)
project(tetrisAI)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES "brains/*" "controller/*" "NEAT.1.2.1/*" "player/*" main.cpp main.h)
add_executable(tetrisAI ${SOURCES})