set(This unittests)

add_executable(${This}
    UnitTests.cpp
)

target_link_libraries(${This} PRIVATE gtest_main)
