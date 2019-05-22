cmake_minimum_required(VERSION 3.9)
project(CAL-Project)

set(CMAKE_CXX_STANDARD 11)
add_executable(CAL-Project main.cpp
        Graph.h
        Location.cpp Location.h
        graphviewer/connection.cpp graphviewer/connection.h graphviewer/edgetype.h graphviewer/graphviewer.cpp graphviewer/graphviewer.h
        )