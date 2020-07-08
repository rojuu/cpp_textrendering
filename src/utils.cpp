#include "utils.hpp"
#include <fstream>
#include <iostream>

uint8_t *readEntireBinaryFile(std::string_view filename)
{
    std::ifstream file(filename.data(), std::ifstream::binary | std::ifstream::ate);

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << filename << "\n";
        return nullptr;
    }

    size_t size = file.tellg();
    file.seekg(file.beg);

    uint8_t *data = new uint8_t[size];

    file.read((char *)data, size);

    return data;
}

void freeBinaryFileContents(uint8_t *contents)
{
    delete[] contents;
}
