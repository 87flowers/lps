#pragma once

#include "lps/generic/vector.def.hpp"
#include "lps/generic/vector_mask.def.hpp"
#include "lps/stdint.hpp"

#include <algorithm>
#include <array>
#include <cstring>

namespace lps::generic {

  template<typename T, usize N>
  constexpr vector<T, N> vector<T, N>::zero() {
    return vector { std::array<T, N> {} };
  }

  template<typename T, usize N>
  constexpr vector<T, N> vector<T, N>::splat(T value) {
    vector v;
    std::fill(v.raw.begin(), v.raw.end(), value);
    return v;
  }

  template<typename T, usize N>
  vector<T, N> vector<T, N>::load(const void* src) {
    vector v;
    std::memcpy(v.raw.data(), src, sizeof(v.raw));
    return v;
  }

  template<typename T, usize N>
  std::array<T, N> vector<T, N>::to_array() const {
    return raw;
  }

}  // namespace lps::generic
