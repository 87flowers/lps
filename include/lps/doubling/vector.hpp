#pragma once

#include "lps/detail/msb.hpp"
#include "lps/doubling/basic_vector_mask.def.hpp"
#include "lps/doubling/vector.def.hpp"
#include "lps/generic/basic_vector_mask.hpp"
#include "lps/generic/vector.hpp"
#include "lps/stdint.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstring>

namespace lps::doubling {

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::vector(const std::array<T, N>& src) :
      raw(std::bit_cast<std::array<Base, 2>>(src)) {
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::zero() {
    vector result;
    result.raw = { Base::zero(), Base::zero() };
    return result;
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::splat(T value) {
    vector result;
    result.raw = { Base::splat(value), Base::splat(value) };
    return result;
  }

  template<class T, usize N, class Base, class Env>
  vector<T, N, Base, Env> vector<T, N, Base, Env>::load(const void* src) {
    vector v;
    std::memcpy(v.raw.data(), src, sizeof(v.raw));
    return v;
  }

  template<class T, usize N, class Base, class Env>
  constexpr T vector<T, N, Base, Env>::read(usize i) const {
    T value;
    std::memcpy(&value, reinterpret_cast<const char*>(raw.data()) + i * sizeof(T), sizeof(T));
    return value;
  }

  template<class T, usize N, class Base, class Env>
  template<class U>
  constexpr Env::template vector<U, std::max(N, 16 / sizeof(U))> vector<T, N, Base, Env>::convert() {
    // TODO
    generic::vector<U, std::max(N, 16 / sizeof(U))> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = static_cast<U>(read(i));
    }
    return std::bit_cast<Env::template vector<U, std::max(N, 16 / sizeof(U))>>(result);
  }

  template<class T, usize N, class Base, class Env>
  template<class V, usize extract_index>
  constexpr V vector<T, N, Base, Env>::extract_aligned() {
    V value;
    std::memcpy(&value, reinterpret_cast<const char*>(raw.data()) + extract_index * sizeof(V), sizeof(V));
    return value;
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::swizzle(const vector<T, N, Base, Env>& src) {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(*this).swizzle(std::bit_cast<generic::vector<T, N>>(src)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::swizzle(const vector<T, N, Base, Env>& src0, const vector<T, N, Base, Env>& src1) {
    return std::bit_cast<vector<T, N, Base, Env>>(
      std::bit_cast<generic::vector<T, N>>(*this).swizzle(std::bit_cast<generic::vector<T, N>>(src0), std::bit_cast<generic::vector<T, N>>(src1)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::swizzle(const mask_type& src) {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).swizzle(std::bit_cast<generic::vector_mask<T, N>>(src)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::swizzle(const Env::template vector<T, 16 / sizeof(T)>& src)
    requires(16 / sizeof(T) != N)
  {
    return std::bit_cast<vector<T, N, Base, Env>>(
      std::bit_cast<generic::vector<T, N>>(*this).swizzle(std::bit_cast<generic::vector<T, 16 / sizeof(T)>>(src)));
  }

  template<class T, usize N, class Base, class Env>
  template<usize shift_amount>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::shl() {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(*this).template shl<shift_amount>());
  }

  template<class T, usize N, class Base, class Env>
  template<usize shift_amount>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::shr() {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(*this).template shr<shift_amount>());
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::andnot(const vector<T, N, Base, Env>& second) const {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(*this).andnot(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr T vector<T, N, Base, Env>::reduce_add() const {
    return std::bit_cast<generic::vector<T, N>>(*this).reduce_add();
  }

  template<class T, usize N, class Base, class Env>
  constexpr T vector<T, N, Base, Env>::reduce_or() const {
    return std::bit_cast<generic::vector<T, N>>(*this).reduce_or();
  }

  template<class T, usize N, class Base, class Env>
  constexpr T vector<T, N, Base, Env>::reduce_xor() const {
    return std::bit_cast<generic::vector<T, N>>(*this).reduce_xor();
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::zip_low(const vector<T, N, Base, Env>& second) const {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(*this).zip_low(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::zip_high(const vector<T, N, Base, Env>& second) const {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(*this).zip_high(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::zip_low_128lanes(const vector<T, N, Base, Env>& second) const {
    return std::bit_cast<vector<T, N, Base, Env>>(
      std::bit_cast<generic::vector<T, N>>(*this).zip_low_128lanes(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::zip_high_128lanes(const vector<T, N, Base, Env>& second) const {
    return std::bit_cast<vector<T, N, Base, Env>>(
      std::bit_cast<generic::vector<T, N>>(*this).zip_high_128lanes(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::test_vm(const vector& second) const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).test_vm(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::test(const vector& second) const {
    return test_vm(second);
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::eq_vm(const vector& second) const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).eq_vm(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::eq(const vector& second) const {
    return eq_vm(second);
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::neq_vm(const vector& second) const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).neq_vm(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::neq(const vector& second) const {
    return neq_vm(second);
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::gt_vm(const vector<T, N, Base, Env>& second) const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).gt_vm(std::bit_cast<generic::vector<T, N>>(second)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::gt(const vector<T, N, Base, Env>& other) const {
    return gt_vm(other);
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::nonzeros_vm() const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).nonzeros_vm());
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::nonzeros() const {
    return nonzeros_vm();
  }

  template<class T, usize N, class Base, class Env>
  constexpr usize vector<T, N, Base, Env>::nonzeros_count() const {
    return std::bit_cast<generic::vector<T, N>>(*this).nonzeros_count();
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::zeros_vm() const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).zeros_vm());
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::zeros() const {
    return zeros_vm();
  }

  template<class T, usize N, class Base, class Env>
  constexpr usize vector<T, N, Base, Env>::zeros_count() const {
    return std::bit_cast<generic::vector<T, N>>(*this).zeros_count();
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::msb_vm() const {
    return std::bit_cast<mask_type>(std::bit_cast<generic::vector<T, N>>(*this).msb_vm());
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::msb() const {
    return msb_vm();
  }

  template<class T, usize N, class Base, class Env>
  std::array<T, N> vector<T, N, Base, Env>::to_array() const {
    return std::bit_cast<std::array<T, N>>(raw);
  }

  template<class T, usize N, class Base, class Env>
  constexpr bool operator==(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first.raw == second.raw;
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> operator~(const vector<T, N, Base, Env>& first) {
    return std::bit_cast<vector<T, N, Base, Env>>(~std::bit_cast<generic::vector<T, N>>(first));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> operator&(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(first) & std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>& operator&=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first & second;
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> operator|(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(first) | std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>& operator|=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first | second;
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> operator^(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(first) ^ std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>& operator^=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first ^ second;
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> operator+(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(first) + std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>& operator+=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first + second;
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> operator-(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(first) - std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>& operator-=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first - second;
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> operator*(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(first) * std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>& operator*=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first * second;
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> operator<<(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(first) << std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>& operator<<=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first << second;
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env> operator>>(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return std::bit_cast<vector<T, N, Base, Env>>(std::bit_cast<generic::vector<T, N>>(first) >> std::bit_cast<generic::vector<T, N>>(second));
  }

  template<class T, usize N, class Base, class Env>
  constexpr vector<T, N, Base, Env>& operator>>=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first >> second;
  }

}  // namespace lps::doubling
