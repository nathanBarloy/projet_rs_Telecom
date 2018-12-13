cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)                     # setting the language standard
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc") #Pour obtenir un .exe sur Windows

project(rs2018 C)         # Indique le language du projet


set(SOURCE_FILES src/rsfind.c src/functions.c headers/functions.h src/misc.c headers/misc.h src/directory.c headers/directory.h src/file.c headers/file.h src/launch.c headers/launch.h)

add_executable(rs2018 ${SOURCE_FILES})


# set the path to the library folder
link_directories(/usr/local/lib)

## link the libraries to the executable
#target_link_libraries (rs2018 magic) # ANCIEN LINK : avant l'utilisation de libdl
target_link_libraries(rs2018 dl)