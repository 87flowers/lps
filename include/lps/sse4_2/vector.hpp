#pragma once

#include "lps/detail/msb.hpp"
#include "lps/generic/basic_vector_mask.def.hpp"
#include "lps/generic/vector.def.hpp"
#include "lps/sse4_2/basic_vector_mask.def.hpp"
#include "lps/sse4_2/vector.def.hpp"
#include "lps/stdint.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstring>
#include <type_traits>

namespace lps::sse4_2 {

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::vector(__m128i src) :
      raw(src) {
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::vector(const std::array<T, N>& src) :
      raw(std::bit_cast<__m128i>(src)) {
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> vector<T, N, Env>::zero() {
    vector result;
    result.raw = _mm_setzero_si128();
    return result;
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> vector<T, N, Env>::splat(T value) {
    vector result;
    if constexpr (sizeof(T) == sizeof(u8)) {
      result.raw = _mm_set1_epi8(std::bit_cast<i8>(value));
    } else if constexpr (sizeof(T) == sizeof(u16)) {
      result.raw = _mm_set1_epi16(std::bit_cast<i16>(value));
    } else if constexpr (sizeof(T) == sizeof(u32)) {
      result.raw = _mm_set1_epi32(std::bit_cast<i32>(value));
    } else if constexpr (sizeof(T) == sizeof(u64)) {
      result.raw = _mm_set1_epi64x(std::bit_cast<i64>(value));
    } else {
      static_assert(false);
    }
    return result;
  }

  template<class T, usize N, class Env>
  vector<T, N, Env> vector<T, N, Env>::load(const void* src) {
    vector result;
    result.raw = _mm_loadu_si128(reinterpret_cast<__m128i const*>(src));
    return result;
  }

  template<class T, usize N, class Env>
  constexpr T vector<T, N, Env>::read(usize i) const {
    T value;
    std::memcpy(&value, reinterpret_cast<const char*>(&raw) + i * sizeof(T), sizeof(T));
    return value;
  }

  template<class T, usize N, class Env>
  template<class U>
  constexpr Env::template vector<U, std::max(N, 16 / sizeof(U))> vector<T, N, Env>::convert() {
    using Result = Env::template vector<U, std::max(N, 16 / sizeof(U))>;
    if constexpr (sizeof(T) == sizeof(u8)) {
      if constexpr (sizeof(U) == sizeof(u8)) {
        return *this;
      } else if constexpr (std::is_same_v<T, i8> && std::is_same_v<U, i16>) {
        return Result { _mm_cvtepi8_epi16(raw) };
      } else if constexpr (sizeof(U) == sizeof(u16)) {
        return Result { _mm_cvtepu8_epi16(raw) };
      } else if constexpr (std::is_same_v<T, i8> && std::is_same_v<U, i32>) {
        return Result { _mm_cvtepi8_epi32(raw) };
      } else if constexpr (sizeof(U) == sizeof(u32)) {
        return Result { _mm_cvtepu8_epi32(raw) };
      } else if constexpr (std::is_same_v<T, i8> && std::is_same_v<U, i64>) {
        return Result { _mm_cvtepi8_epi64(raw) };
      } else if constexpr (sizeof(U) == sizeof(u64)) {
        return Result { _mm_cvtepu8_epi64(raw) };
      } else {
        static_assert(false);
      }
    } else if constexpr (sizeof(T) == sizeof(u16)) {
      if constexpr (sizeof(U) == sizeof(u8)) {
        // TODO: Optimize
        return Result { _mm_packus_epi16(_mm_and_si128(raw, _mm_set1_epi16(0x00FF)), _mm_setzero_si128()) };
      } else if constexpr (sizeof(U) == sizeof(u16)) {
        return *this;
      } else if constexpr (std::is_same_v<T, i16> && std::is_same_v<U, i32>) {
        return Result { _mm_cvtepi16_epi32(raw) };
      } else if constexpr (sizeof(U) == sizeof(u32)) {
        return Result { _mm_cvtepu16_epi32(raw) };
      } else if constexpr (std::is_same_v<T, i16> && std::is_same_v<U, i64>) {
        return Result { _mm_cvtepi16_epi64(raw) };
      } else if constexpr (sizeof(U) == sizeof(u64)) {
        return Result { _mm_cvtepu16_epi64(raw) };
      } else {
        static_assert(false);
      }
    } else if constexpr (sizeof(T) == sizeof(u32)) {
      if constexpr (sizeof(U) == sizeof(u8)) {
        // TODO: Optimize
        return Result { _mm_packus_epi16(_mm_packus_epi32(_mm_and_si128(raw, _mm_set1_epi32(0x00FF)), _mm_setzero_si128()), _mm_setzero_si128()) };
      } else if constexpr (sizeof(U) == sizeof(u16)) {
        // TODO: Optimize
        return Result { _mm_packus_epi32(_mm_and_si128(raw, _mm_set1_epi32(0x0000FFFF)), _mm_setzero_si128()) };
      } else if constexpr (sizeof(U) == sizeof(u32)) {
        return *this;
      } else if constexpr (std::is_same_v<T, i32> && std::is_same_v<U, i64>) {
        return Result { _mm_cvtepi32_epi64(raw) };
      } else if constexpr (sizeof(U) == sizeof(u64)) {
        return Result { _mm_cvtepu32_epi64(raw) };
      } else {
        static_assert(false);
      }
    } else if constexpr (sizeof(T) == sizeof(u64)) {
      if constexpr (sizeof(U) == sizeof(u8)) {
        // TODO: Implement
        static_assert(false, "unimplemented");
      } else if constexpr (sizeof(U) == sizeof(u16)) {
        // TODO: Implement
        static_assert(false, "unimplemented");
      } else if constexpr (sizeof(U) == sizeof(u32)) {
        // TODO: Implement
        static_assert(false, "unimplemented");
      } else if constexpr (sizeof(U) == sizeof(u64)) {
        return *this;
      } else {
        static_assert(false);
      }
    } else {
      static_assert(false);
    }
  }

  template<class T, usize N, class Env>
  template<class V, usize extract_index>
  constexpr V vector<T, N, Env>::extract_aligned() {
    V value;
    std::memcpy(&value, reinterpret_cast<const char*>(&raw) + extract_index * sizeof(V), sizeof(V));
    return value;
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> vector<T, N, Env>::swizzle(const vector<T, N, Env>& src) {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(*this).swizzle(std::bit_cast<generic::vector<T, N>>(src)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> vector<T, N, Env>::swizzle(const vector<T, N, Env>& src0, const vector<T, N, Env>& src1) {
    return std::bit_cast<vector<T, N, Env>>(
      std::bit_cast<generic::vector<T, N>>(*this).swizzle(std::bit_cast<generic::vector<T, N>>(src0), std::bit_cast<generic::vector<T, N>>(src1)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::swizzle(const mask_type& src) {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).swizzle(std::bit_cast<generic::vector_mask<T, N>>(src)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> vector<T, N, Env>::swizzle(const Env::template vector<T, 16 / sizeof(T)>& src)
    requires(16 / sizeof(T) != N)
  {
    return std::bit_cast<vector<T, N, Env>>(
      std::bit_cast<generic::vector<T, N>>(*this).swizzle(std::bit_cast<generic::vector<T, 16 / sizeof(T)>>(src)));
  }

  template<class T, usize N, class Env>
  template<usize shift_amount>
  constexpr vector<T, N, Env> vector<T, N, Env>::shl() {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(*this).template shl<shift_amount>());
  }

  template<class T, usize N, class Env>
  template<usize shift_amount>
  constexpr vector<T, N, Env> vector<T, N, Env>::shr() {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(*this).template shr<shift_amount>());
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> vector<T, N, Env>::andnot(const vector<T, N, Env>& second) const {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(*this).andnot(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Env>
  constexpr T vector<T, N, Env>::reduce_add() const {
    return std::bit_cast<generic::vector<T, N>>(*this).reduce_add();
  }

  template<class T, usize N, class Env>
  constexpr T vector<T, N, Env>::reduce_or() const {
    return std::bit_cast<generic::vector<T, N>>(*this).reduce_or();
  }

  template<class T, usize N, class Env>
  constexpr T vector<T, N, Env>::reduce_xor() const {
    return std::bit_cast<generic::vector<T, N>>(*this).reduce_xor();
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> vector<T, N, Env>::zip_low(const vector<T, N, Env>& second) const {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(*this).zip_low(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> vector<T, N, Env>::zip_high(const vector<T, N, Env>& second) const {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(*this).zip_high(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> vector<T, N, Env>::zip_low_128lanes(const vector<T, N, Env>& second) const {
    return std::bit_cast<vector<T, N, Env>>(
      std::bit_cast<generic::vector<T, N>>(*this).zip_low_128lanes(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> vector<T, N, Env>::zip_high_128lanes(const vector<T, N, Env>& second) const {
    return std::bit_cast<vector<T, N, Env>>(
      std::bit_cast<generic::vector<T, N>>(*this).zip_high_128lanes(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::test_vm(const vector& second) const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).test_vm(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::test(const vector& second) const {
    return test_vm(second);
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::eq_vm(const vector& second) const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).eq_vm(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::eq(const vector& second) const {
    return eq_vm(second);
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::neq_vm(const vector& second) const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).neq_vm(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::neq(const vector& second) const {
    return neq_vm(second);
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::gt_vm(const vector<T, N, Env>& second) const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).gt_vm(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::gt(const vector<T, N, Env>& other) const {
    return gt_vm(other);
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::nonzeros_vm() const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).nonzeros_vm());
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::nonzeros() const {
    return nonzeros_vm();
  }

  template<class T, usize N, class Env>
  constexpr usize vector<T, N, Env>::nonzeros_count() const {
    return std::bit_cast<generic::vector<T, N>>(*this).nonzeros_count();
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::zeros_vm() const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).zeros_vm());
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::zeros() const {
    return zeros_vm();
  }

  template<class T, usize N, class Env>
  constexpr usize vector<T, N, Env>::zeros_count() const {
    return std::bit_cast<generic::vector<T, N>>(*this).zeros_count();
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::msb_vm() const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).msb_vm());
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>::mask_type vector<T, N, Env>::msb() const {
    return msb_vm();
  }

  template<class T, usize N, class Env>
  std::array<T, N> vector<T, N, Env>::to_array() const {
    return std::bit_cast<std::array<T, N>>(raw);
  }

  template<class T, usize N, class Env>
  constexpr bool operator==(const vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    __m128i x = _mm_xor_si128(first.raw, second.raw);
    return _mm_testz_si128(x, x);
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> operator~(const vector<T, N, Env>& first) {
    return std::bit_cast<vector<T, N, Env>>(~std::bit_cast<generic::vector<T, N>>(first));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> operator&(const vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(first) & std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>& operator&=(vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return first = first & second;
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> operator|(const vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(first) | std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>& operator|=(vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return first = first | second;
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> operator^(const vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(first) ^ std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>& operator^=(vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return first = first ^ second;
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> operator+(const vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(first) + std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>& operator+=(vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return first = first + second;
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> operator-(const vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(first) - std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>& operator-=(vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return first = first - second;
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> operator*(const vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(first) * std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>& operator*=(vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return first = first * second;
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> operator<<(const vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(first) << std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>& operator<<=(vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return first = first << second;
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env> operator>>(const vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return std::bit_cast<vector<T, N, Env>>(std::bit_cast<generic::vector<T, N>>(first) >> std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Env>
  constexpr vector<T, N, Env>& operator>>=(vector<T, N, Env>& first, const vector<T, N, Env>& second) {
    return first = first >> second;
  }

}  // namespace lps::sse4_2
