#pragma once

#include "Common.hpp"
#include <iostream>

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

inline std::string readEntireTextFile(const char *filename)
{
    std::string result;

    std::ifstream file(filename, std::ifstream::ate);

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << "\n";
        return result;
    }

    size_t size = file.tellg();
    file.seekg(std::ifstream::beg);

    result.resize(size);
    file.read(result.data(), size);

    return result;
}
