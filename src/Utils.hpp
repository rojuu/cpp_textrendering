#pragma once

#include "Types.hpp"
#include <fmt/format.h>

template <typename Func>
struct DeferWrapper {
    Func f;
    DeferWrapper(Func f)
        : f(f)
    {
    }
    ~DeferWrapper() { f(); }
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

uint8_t *readEntireBinaryFile(const char *filename);
void freeBinaryFileContents(uint8_t *contents);

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