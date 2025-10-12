#pragma once

#include "lps/generic/basic_vector_mask.def.hpp"
#include "lps/generic/vector.def.hpp"
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
  constexpr vector_mask<T, N> vector<T, N>::gt_vm(const vector<T, N>& other) const {
    vector_mask<T, N> m;
    for (usize i = 0; i < N; i++) {
      m.set(i, raw[i] > other.raw[i]);
    }
    return m;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::gt(const vector<T, N>& other) const {
    return gt_vm(other);
  }

  template<typename T, usize N>
  std::array<T, N> vector<T, N>::to_array() const {
    return raw;
  }

  template<typename T, usize N>
  constexpr vector<T, N> operator+(const vector<T, N>& first, const vector<T, N>& second) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = first.raw[i] + second.raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N>& operator+=(vector<T, N>& first, const vector<T, N>& second) {
    for (usize i = 0; i < N; i++) {
      first.raw[i] += second.raw[i];
    }
    return first;
  }

}  // namespace lps::generic
