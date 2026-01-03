cmake_minimum_required(VERSION 3.31)
project(demoCppProjectRemoveFIles)

set(CMAKE_CXX_STANDARD 20)

add_executable(demoCppProjectRemoveFIles main.cpp
        helpers/headers/helper.h
        helpers/headers/console/HConsole.h
        helpers/cpp/helper.cpp
        helpers/cpp/console/HConsole.cpp
)
