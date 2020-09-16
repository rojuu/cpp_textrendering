set(This unittests)

add_executable(${This}
    Utils.cpp
)

add_test(NAME ${This} COMMAND ${This})

target_include_directories(${This} PRIVATE ../src)
target_link_libraries(${This} PRIVATE fmt)
target_link_libraries(${This} PRIVATE gtest_main)