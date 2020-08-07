#pragma once

#include <fmt/format.h>

#include <cstdint>
#include <fstream>
#include <iostream>

template <typename Func>
class DeferWrapper {
    Func f;

public:
    explicit DeferWrapper(Func f)
        : f(f)
    {
    }
    ~DeferWrapper() { f(); }

    DeferWrapper(const DeferWrapper &) = delete;
    DeferWrapper &operator=(const DeferWrapper &) = delete;
    DeferWrapper(DeferWrapper &&) = delete;
    DeferWrapper &operator=(DeferWrapper &&) = delete;
};

template <typename Func>
DeferWrapper<Func> deferFunc(Func f)
{
    return DeferWrapper<Func>(f);
}

#define CONCAT2(x, y) x##y
#define CONCAT(x, y) CONCAT2(x, y)
#define DEFER_C(x) CONCAT(x, __LINE__)
#define defer(code)                                                                                \
    auto DEFER_C(_defer_) = deferFunc([&]() {                                                      \
        code;                                                                                      \
    })

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