#pragma once

#include <cstdint>
#include <fmt/format.h>
#include <fstream>
#include <iostream>

inline uint8_t *readEntireBinaryFile(const char *filename)
{
    std::ifstream file(filename, std::ifstream::binary | std::ifstream::ate);

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << "\n";
        return nullptr;
    }

    size_t size = file.tellg();
    file.seekg(std::ifstream::beg);

    auto *data = new uint8_t[size];
    file.read(reinterpret_cast<char *>(data), size);

    return data;
}

inline void freeBinaryFileContents(const uint8_t *contents)
{
    delete[] contents;
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