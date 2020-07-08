set(This unittests)

add_executable(${This}
    utils.cpp
)

target_link_libraries(${This} PRIVATE gtest_main)
