cmake_minimum_required(VERSION 4.1)
project(C__)

set(CMAKE_CXX_STANDARD 20)

add_executable(C__
        main.cpp
        evenorodd.cpp
        evenorodd.h
        TicTacToe.cpp
        Project/TicTacToe.h
        Project/er_triage.h
        Project/intrusion_detection.cpp
        pointers.cpp
        Project/dynamic_array.cpp
        Project/dynamic_array.h
        Project/struct.cpp
        Project/struct.h
        Project/arraysum.cpp
        Project/arraysum.h
        Project/log_analyzer.cpp
        Project/log_analyzer.h
        Project/log_analyzer.cpp
)

add_subdirectory(website-checker)
