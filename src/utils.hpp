#pragma once

#include "common.hpp"
#include <vector>
#include <sstream>

template <typename T>
class Maybe {
    T m_value;
    bool m_hasValue;

public:
    Maybe()
        : m_hasValue(false)
    {
    }
    Maybe(T value)
        : m_value(value)
        , m_hasValue(true)
    {
    }
    Maybe(Maybe &other)
    {
        m_value = other.m_value;
        m_hasValue = other.m_hasValue;
    }
    Maybe &operator=(Maybe &other)
    {
        m_value = other.m_value;
        m_hasValue = other.m_hasValue;
    }
};

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
