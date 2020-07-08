#pragma once

#include "types.hpp"
#include <sstream>

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

inline std::string format(const char *fmt)
{
    return fmt;
}

template <typename T, typename... Trest>
std::string format(const char *_fmt, T value, Trest... rest)
{
    auto *fmt = _fmt;
    std::stringstream ss;
    for (; *fmt != '\0'; fmt++) {
        if (*fmt == '%') {
            ss << value;
            ss << format(fmt + 1, rest...);
            return ss.str();
        }
        ss << *fmt;
    }
    return ss.str();
}
