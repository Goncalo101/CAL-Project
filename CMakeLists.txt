cmake_minimum_required(VERSION 3.9)
project(CAL-Project)

set(CMAKE_CXX_STANDARD 11)
add_executable(CAL-Project main.cpp
        Graph.h
        Location.cpp Location.h
        connection.cpp connection.h edgetype.h graphviewer.cpp graphviewer.h
        )