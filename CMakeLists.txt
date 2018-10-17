cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)                     # setting the language standard
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc") #Pour obtenir un .exe sur Windows

project(rs2018 C)         # Indique le language du projet


set(SOURCE_FILES src/rsfind.c src/functions.c headers/functions.h)

add_executable(rs2018 ${SOURCE_FILES})