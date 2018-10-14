cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)                     # setting the language standard

project(rs2018 C)         # Indique le language du projet


set(SOURCE_FILES rsfind.c)

add_executable(PCSD ${SOURCE_FILES})