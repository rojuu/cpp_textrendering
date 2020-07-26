cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(This rojueditor)
project(${This})

add_subdirectory(deps)
add_subdirectory(tests)

add_executable(${This}
    src/main.cpp
)

set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)
target_link_libraries(${This} PRIVATE SDL2)
target_link_libraries(${This} PRIVATE fmt)
target_include_directories(${This} PRIVATE deps/include)
