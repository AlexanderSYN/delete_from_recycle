cmake_minimum_required(VERSION 3.31)

project(demoCppProjectRemoveFIles)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

add_executable(demoCppProjectRemoveFIles
        main.cpp
        helpers/headers/helper.h
        helpers/headers/console/HConsole.h
        helpers/headers/work_with_recycle/HWork_with_recycle.h
        helpers/cpp/helper.cpp
        helpers/cpp/console/HConsole.cpp
        helpers/cpp/work_with_recycle/HWork_with_recycle.cpp
)

target_link_libraries(demoCppProjectRemoveFIles
        ole32
        shell32
)
