cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(This rojuide)
project(${This})

add_executable(${This}
    src/common.h
    src/utils.h
    src/utils.c
    src/renderer.h
    src/renderer.c
    src/stb_ds.h
    src/stb_ds.c
    src/stb_truetype.h
    src/stb_truetype.c
    src/main.c
)

add_subdirectory(deps/SDL2)

target_link_libraries(${This} PRIVATE SDL2)
