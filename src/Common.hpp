#include <cstdint>
#include <exception>
#include <fmt/format.h>
#include <iostream>
#include <stdexcept>
#include <vector>

#define DEFAULT_MOVEABLE(C) \
    C(C &&) = default;      \
    C &operator=(C &&) = default;
#define DEFAULT_COPYABLE(C) \
    C(const C &) = default; \
    C &operator=(const C &) = default

#define DELETE_MOVEABLE(C) \
    C(C &&) = delete;      \
    C &operator=(C &&) = delete
#define DELETE_COPYABLE(C) \
    C(const C &) = delete; \
    C &operator=(const C &) = delete

#define DEFAULT_COPYABLE_AND_MOVEABLE(C) \
    DEFAULT_MOVEABLE(C);                 \
    DEFAULT_COPYABLE(C)

#define DELETE_COPYABLE_AND_MOVEABLE(C) \
    DELETE_MOVEABLE(C);                 \
    DELETE_COPYABLE(C)

#define LOG() std::cout
#define LOG_DEBUG() std::cout
#define LOG_ERR() std::cerr
