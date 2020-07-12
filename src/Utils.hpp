#pragma once

#include "Types.hpp"
#include <iostream>

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

inline void streamPrint(std::ostream &stream, const char *fmt)
{
    stream << fmt;
}

template <typename T, typename... TRest>
void streamPrint(std::ostream &stream, const char *fmt, T value, TRest... rest)
{
    for (; *fmt != '\0'; ++fmt) {
        if (*fmt == '%') {
            stream << value;
            streamPrint(stream, fmt + 1, rest...);
            return;
        }
        stream << *fmt;
    }
}

template <typename... TRest>
void print(const char *fmt, TRest... rest)
{
    streamPrint(std::cout, fmt, rest...);
}

template <typename... TRest>
void printErr(const char *fmt, TRest... rest)
{
    streamPrint(std::cerr, fmt, rest...);
}