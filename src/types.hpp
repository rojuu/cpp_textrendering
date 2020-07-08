#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include <unordered_map>

template <typename T>
using Optional = std::optional<T>;

template <typename T>
using DynArray = std::vector<T>;

template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;
