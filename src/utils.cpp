#include "utils.hpp"
#include <fstream>
#include <iostream>

bool ScopedBinaryFile::readEntireFile(const char *filename)
{
    std::ifstream file(filename, std::ifstream::binary | std::ifstream::ate);

    if (!file.is_open()) {
        std::cerr << format("Failed to open file %\n", filename);
        return false;
    }

    size_t size = file.tellg();
    file.seekg(file.beg);

    m_data.clear();
    m_data.reserve(size);

    file.read((char *)m_data.data(), size);

    return true;
}
