cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(This rojueditor)
project(${This})

add_subdirectory(deps)
add_subdirectory(tests)

add_executable(${This}
    src/Types.hpp
    src/Utils.hpp
    src/Utils.cpp
    src/Renderer.hpp
    src/Renderer.cpp
    src/OLHashMap.hpp
    src/stb_truetype.h
    src/stb_truetype.cpp
    src/main.cpp
)

set_property(TARGET ${This} PROPERTY CXX_STANDARD 17)
target_link_libraries(${This} PRIVATE SDL2)
target_compile_options(${This} PRIVATE -fno-cxx-exceptions -fno-rtti)
