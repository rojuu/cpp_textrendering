cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(This rojueditor)
project(${This})

add_subdirectory(deps)
add_subdirectory(tests)

add_executable(${This}
    src/main.cpp
    src/Renderer.cpp
    src/stb_truetype.c
)

set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)
target_compile_options(${This} PUBLIC -fno-exceptions -fno-rtti -Wall -Wextra -Wpedantic)
target_link_libraries(${This} PRIVATE SDL2)
target_link_libraries(${This} PRIVATE fmt)
target_include_directories(${This} PRIVATE deps/include)
