#pragma once

#include <cstdint>
#include <exception>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <stdexcept>

inline std::vector<uint8_t> readEntireBinaryFile(const char *filename)
{
    std::vector<uint8_t> result;

    std::ifstream file(filename, std::ifstream::binary | std::ifstream::ate);

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << "\n";
        return result;
    }

    size_t size = file.tellg();
    file.seekg(std::ifstream::beg);

    result.resize(size);
    // NOLINTNEXTLINE
    file.read(reinterpret_cast<char *>(result.data()), size);

    return result;
}

template <typename... Args>
void print(const char *fmt, Args... rest)
{
    fmt::print(stdout, fmt, std::forward<Args>(rest)...);
}

template <typename... Args>
void printErr(const char *fmt, Args... rest)
{
    fmt::print(stderr, fmt, std::forward<Args>(rest)...);
}