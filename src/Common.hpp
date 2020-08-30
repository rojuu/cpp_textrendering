#include <cstdint>
#include <fmt/format.h>

// TODO: Don't use STL containers to avoid unhandled bad_alloc exceptions
// as we compile with -fno-exceptions
#include <vector>

#define DEFAULT_MOVEABLE(C) \
    C(C &&) = default;      \
    C &operator=(C &&) = default;
#define DEFAULT_COPYABLE(C) \
    C(const C &) = default; \
    C &operator=(const C &) = default;

#define DELETE_MOVEABLE(C) \
    C(C &&) = delete;      \
    C &operator=(C &&) = delete;
#define DELETE_COPYABLE(C) \
    C(const C &) = delete; \
    C &operator=(const C &) = delete;

#define DEFAULT_COPYABLE_AND_MOVEABLE(C) \
    DEFAULT_MOVEABLE(C);                 \
    DEFAULT_COPYABLE(C)

#define DELETE_COPYABLE_AND_MOVEABLE(C) \
    DELETE_MOVEABLE(C);                 \
    DELETE_COPYABLE(C)
