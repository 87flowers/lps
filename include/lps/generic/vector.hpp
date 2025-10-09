#pragma once

#include <algorithm>
#include <array>
#include <cstring>

#include "lps/stdint.hpp"

namespace lps::generic {

template <class T, usize N> struct vector_mask;

template <class T, usize N> struct vector {
  std::array<T, N> raw;

  static constexpr usize size = N;

  explicit constexpr vector() = default;
  explicit constexpr vector(const std::array<T, N> &src) : raw(src) {}

  static constexpr vector zero() { return vector{std::array<T, N>{}}; }
  static constexpr vector splat(T value) {
    vector v;
    std::fill(v.raw.begin(), v.raw.end(), value);
    return v;
  }
  static vector load(const void *src) {
    vector v;
    std::memcpy(v.raw.data(), src, sizeof(v.raw));
    return v;
  }

  [[nodiscard]] std::array<T, N> to_array() const { return raw; }
};

} // namespace lps::generic
