#pragma once

#include "common.hpp"
#include <vector>

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

class BinaryFile {
    std::vector<uint8_t> m_data;

public:
    bool readEntireFile(const char *filename);
    const uint8_t *data() const { return m_data.data(); }
};
