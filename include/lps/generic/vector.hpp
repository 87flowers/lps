#pragma once

#include "lps/detail/msb.hpp"
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
  constexpr T vector<T, N>::read(usize i) const {
    return raw[i];
  }

  template<typename T, usize N>
  template<typename U>
  constexpr vector<U, std::max(N, 16 / sizeof(U))> vector<T, N>::convert() {
    vector<U, std::max(N, 16 / sizeof(U))> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = static_cast<U>(raw[i]);
    }
    return result;
  }

  template<typename T, usize N>
  template<class V, usize extract_index>
  constexpr V vector<T, N>::extract_aligned() {
    V result;
    for (usize i = 0; i < V::size; i++) {
      result.raw[i] = raw[extract_index * V::size + i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N> vector<T, N>::swizzle(const vector<T, N>& src) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = raw[i] < N ? src.raw[raw[i]] : 0;
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N> vector<T, N>::swizzle(const vector<T, N>& src0, const vector<T, N>& src1) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = raw[i] < 2 * N ? (raw[i] < N ? src0 : src1).raw[raw[i]] : 0;
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::swizzle(const vector_mask<T, N>& src) {
    vector_mask<T, N> result;
    result.raw = swizzle(src.raw);
    return result;
  }

  template<typename T, usize N>
  template<usize M>
    requires(M != N)
  constexpr vector<T, N> vector<T, N>::swizzle(const vector<T, M>& src) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = raw[i] < M ? src.raw[raw[i]] : 0;
    }
    return result;
  }

  template<typename T, usize N>
  template<usize shift_amount>
  constexpr vector<T, N> vector<T, N>::shl() {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = raw[i] << shift_amount;
    }
    return result;
  }

  template<typename T, usize N>
  template<usize shift_amount>
  constexpr vector<T, N> vector<T, N>::shr() {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = raw[i] >> shift_amount;
    }
    return result;
  }

  template<typename T, usize N>
  constexpr T vector<T, N>::reduce_add() const {
    T result = 0;
    for (usize i = 0; i < N; i++) {
      result += raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr T vector<T, N>::reduce_or() const {
    T result = 0;
    for (usize i = 0; i < N; i++) {
      result |= raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr T vector<T, N>::reduce_xor() const {
    T result = 0;
    for (usize i = 0; i < N; i++) {
      result ^= raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N> vector<T, N>::zip_low(const vector<T, N>& second) const {
    static_assert(N % 2 == 0);
    vector<T, N> result;
    for (usize i = 0; i < N; i += 2) {
      result.raw[i + 0] = raw[i / 2];
      result.raw[i + 1] = second.raw[i / 2];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N> vector<T, N>::zip_high(const vector<T, N>& second) const {
    static_assert(N % 2 == 0);
    vector<T, N> result;
    for (usize i = 0; i < N; i += 2) {
      result.raw[i + 0] = raw[(N + i) / 2];
      result.raw[i + 1] = second.raw[(N + i) / 2];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N> vector<T, N>::zip_low_128lanes(const vector<T, N>& second) const {
    static_assert(N % 2 == 0);
    vector<T, N> result;
    constexpr usize lane_width = 16 / sizeof(T);
    constexpr usize lane_count = N / lane_width;
    static_assert(lane_count * lane_width == N);
    for (usize lane = 0; lane < lane_count; lane++) {
      usize lane_start = lane * lane_width;
      for (usize i = 0; i < lane_width; i += 2) {
        result.raw[lane_start + i + 0] = raw[lane_start + i / 2];
        result.raw[lane_start + i + 1] = second.raw[lane_start + i / 2];
      }
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N> vector<T, N>::zip_high_128lanes(const vector<T, N>& second) const {
    static_assert(N % 2 == 0);
    vector<T, N> result;
    constexpr usize lane_width = 16 / sizeof(T);
    constexpr usize lane_count = N / lane_width;
    static_assert(lane_count * lane_width == N);
    for (usize lane = 0; lane < lane_count; lane++) {
      usize lane_start = lane * lane_width;
      for (usize i = 0; i < lane_width; i += 2) {
        result.raw[lane_start + i + 0] = raw[lane_start + (lane_width + i) / 2];
        result.raw[lane_start + i + 1] = second.raw[lane_start + (lane_width + i) / 2];
      }
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::test_vm(const vector& second) const {
    vector_mask<T, N> m = vector_mask<T, N>::zero();
    for (usize i = 0; i < N; i++) {
      m.set(i, raw[i] & second.raw[i]);
    }
    return m;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::test(const vector& second) const {
    return test_vm(second);
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::eq_vm(const vector& second) const {
    vector_mask<T, N> m = vector_mask<T, N>::zero();
    for (usize i = 0; i < N; i++) {
      m.set(i, raw[i] == second.raw[i]);
    }
    return m;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::eq(const vector& second) const {
    return eq_vm(second);
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::neq_vm(const vector& second) const {
    vector_mask<T, N> m = vector_mask<T, N>::zero();
    for (usize i = 0; i < N; i++) {
      m.set(i, raw[i] != second.raw[i]);
    }
    return m;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::neq(const vector& second) const {
    return neq_vm(second);
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::gt_vm(const vector<T, N>& second) const {
    vector_mask<T, N> m = vector_mask<T, N>::zero();
    for (usize i = 0; i < N; i++) {
      m.set(i, raw[i] > second.raw[i]);
    }
    return m;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::gt(const vector<T, N>& other) const {
    return gt_vm(other);
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::nonzeros_vm() const {
    vector_mask<T, N> m = vector_mask<T, N>::zero();
    for (usize i = 0; i < N; i++) {
      m.set(i, raw[i] != 0);
    }
    return m;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::nonzeros() const {
    return nonzeros_vm();
  }

  template<typename T, usize N>
  constexpr usize vector<T, N>::nonzeros_count() const {
    usize result = 0;
    for (usize i = 0; i < N; i++) {
      result += raw[i] != 0;
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::zeros_vm() const {
    vector_mask<T, N> m = vector_mask<T, N>::zero();
    for (usize i = 0; i < N; i++) {
      m.set(i, raw[i] == 0);
    }
    return m;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::zeros() const {
    return zeros_vm();
  }

  template<typename T, usize N>
  constexpr usize vector<T, N>::zeros_count() const {
    usize result = 0;
    for (usize i = 0; i < N; i++) {
      result += raw[i] == 0;
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::msb_vm() const {
    vector_mask<T, N> m = vector_mask<T, N>::zero();
    for (usize i = 0; i < N; i++) {
      m.set(i, detail::msb(raw[i]));
    }
    return m;
  }

  template<typename T, usize N>
  constexpr vector_mask<T, N> vector<T, N>::msb() const {
    return msb_vm();
  }

  template<typename T, usize N>
  std::array<T, N> vector<T, N>::to_array() const {
    return raw;
  }

  template<typename T, usize N>
  constexpr bool operator==(const vector<T, N>& first, const vector<T, N>& second) {
    return first.raw == second.raw;
  }

  template<typename T, usize N>
  constexpr vector<T, N> andnot(const vector<T, N>& first, const vector<T, N>& second) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = ~first.raw[i] & second.raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N> operator~(const vector<T, N>& first) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = ~first.raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N> operator&(const vector<T, N>& first, const vector<T, N>& second) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = first.raw[i] & second.raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N>& operator&=(vector<T, N>& first, const vector<T, N>& second) {
    for (usize i = 0; i < N; i++) {
      first.raw[i] &= second.raw[i];
    }
    return first;
  }

  template<typename T, usize N>
  constexpr vector<T, N> operator|(const vector<T, N>& first, const vector<T, N>& second) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = first.raw[i] | second.raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N>& operator|=(vector<T, N>& first, const vector<T, N>& second) {
    for (usize i = 0; i < N; i++) {
      first.raw[i] |= second.raw[i];
    }
    return first;
  }

  template<typename T, usize N>
  constexpr vector<T, N> operator^(const vector<T, N>& first, const vector<T, N>& second) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = first.raw[i] ^ second.raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N>& operator^=(vector<T, N>& first, const vector<T, N>& second) {
    for (usize i = 0; i < N; i++) {
      first.raw[i] ^= second.raw[i];
    }
    return first;
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

  template<typename T, usize N>
  constexpr vector<T, N> operator-(const vector<T, N>& first, const vector<T, N>& second) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = first.raw[i] - second.raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N>& operator-=(vector<T, N>& first, const vector<T, N>& second) {
    for (usize i = 0; i < N; i++) {
      first.raw[i] -= second.raw[i];
    }
    return first;
  }

  template<typename T, usize N>
  constexpr vector<T, N> operator*(const vector<T, N>& first, const vector<T, N>& second) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = first.raw[i] * second.raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N>& operator*=(vector<T, N>& first, const vector<T, N>& second) {
    for (usize i = 0; i < N; i++) {
      first.raw[i] *= second.raw[i];
    }
    return first;
  }

  template<typename T, usize N>
  constexpr vector<T, N> operator<<(const vector<T, N>& first, const vector<T, N>& second) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = first.raw[i] << second.raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N>& operator<<=(vector<T, N>& first, const vector<T, N>& second) {
    for (usize i = 0; i < N; i++) {
      first.raw[i] <<= second.raw[i];
    }
    return first;
  }

  template<typename T, usize N>
  constexpr vector<T, N> operator>>(const vector<T, N>& first, const vector<T, N>& second) {
    vector<T, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = first.raw[i] >> second.raw[i];
    }
    return result;
  }

  template<typename T, usize N>
  constexpr vector<T, N>& operator>>=(vector<T, N>& first, const vector<T, N>& second) {
    for (usize i = 0; i < N; i++) {
      first.raw[i] >>= second.raw[i];
    }
    return first;
  }

}  // namespace lps::generic
