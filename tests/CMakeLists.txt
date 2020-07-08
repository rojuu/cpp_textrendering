set(This unittests)

add_executable(${This}
    OLHashMap.cpp
)

target_include_directories(${This} PRIVATE ../src)
target_link_libraries(${This} PRIVATE gtest_main)