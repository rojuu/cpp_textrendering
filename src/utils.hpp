#pragma once

#include "types.hpp"

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

uint8_t *readEntireBinaryFile(std::string_view filename);
void freeBinaryFileContents(uint8_t *contents);
