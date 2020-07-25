set(This unittests)

enable_testing()

add_executable(${This}
    Containers.cpp
)

add_test(NAME ${This} COMMAND ${This})

target_include_directories(${This} PRIVATE ../src)
target_link_libraries(${This} PRIVATE gtest_main)