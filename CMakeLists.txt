cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(This rojuide)
project(${This})

add_executable(${This}
    src/common.hpp
    src/utils.hpp
    src/utils.cpp
    src/renderer.hpp
    src/renderer.cpp
    src/stb_truetype.h
    src/stb_truetype.cpp
    src/main.cpp
)

add_subdirectory(deps/SDL2)

target_link_libraries(${This} PRIVATE SDL2)
