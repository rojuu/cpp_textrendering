cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(This rojueditor)
project(${This})

add_subdirectory(deps)
add_subdirectory(tests)

set(SOURCES
    src/main.cpp
    src/Renderer.hpp
    src/Renderer.cpp
    src/Utils.hpp
    src/stb_truetype.c
)

add_executable(${This} ${SOURCES})

set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)
target_compile_options(${This} PUBLIC -Wall -Wextra -Wpedantic)
target_link_libraries(${This} PRIVATE SDL2)
target_link_libraries(${This} PRIVATE fmt)
target_include_directories(${This} PRIVATE deps/include)

add_custom_target(clang-tidy
    COMMAND clang-tidy -p ${CMAKE_BUILD_DIR} ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
