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
  LPS_INLINE constexpr vector<T, N, Base, Env>::vector(const std::array<T, N>& src) :
      raw(std::bit_cast<std::array<Base, 2>>(src)) {
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::zero() {
    vector<T, N, Base, Env> result;
    result.raw = { Base::zero(), Base::zero() };
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::splat(T value) {
    vector<T, N, Base, Env> result;
    result.raw = { Base::splat(value), Base::splat(value) };
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::splat(half_vector value) {
    vector<T, N, Base, Env> result;
    result.raw = { value, value };
    return result;
  }

  template<class T, usize N, class Base, class Env>
  vector<T, N, Base, Env> vector<T, N, Base, Env>::load(const void* src) {
    vector v;
    std::memcpy(v.raw.data(), src, sizeof(v.raw));
    return v;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr T vector<T, N, Base, Env>::read(usize i) const {
    T value;
    std::memcpy(&value, reinterpret_cast<const char*>(raw.data()) + i * sizeof(T), sizeof(T));
    return value;
  }

  template<class T, usize N, class Base, class Env>
  template<class U>
  LPS_INLINE constexpr Env::template vector<U, std::max(N, 16 / sizeof(U))> vector<T, N, Base, Env>::convert() {
    // TODO
    generic::vector<U, std::max(N, 16 / sizeof(U))> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = static_cast<U>(read(i));
    }
    return std::bit_cast<Env::template vector<U, std::max(N, 16 / sizeof(U))>>(result);
  }

  template<class T, usize N, class Base, class Env>
  template<class V, usize extract_index>
  LPS_INLINE constexpr V vector<T, N, Base, Env>::extract_aligned() {
    V value;
    std::memcpy(&value, reinterpret_cast<const char*>(raw.data()) + extract_index * sizeof(V), sizeof(V));
    return value;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr std::tuple<typename vector<T, N, Base, Env>::half_vector, typename vector<T, N, Base, Env>::half_vector>
    vector<T, N, Base, Env>::split() const {
    return { raw[0], raw[1] };
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr typename vector<T, N, Base, Env>::dup_vector vector<T, N, Base, Env>::dup() const {
    return dup_vector::splat(*this);
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::swizzle(const vector<T, N, Base, Env>& src) {
    vector<T, N, Base, Env> result;
    result.raw[0] = raw[0].swizzle(src.raw[0], src.raw[1]);
    result.raw[1] = raw[1].swizzle(src.raw[0], src.raw[1]);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::swizzle(const vector<T, N, Base, Env>& src0,
                                                                                const vector<T, N, Base, Env>& src1) {
    static_assert(std::has_single_bit(N));

    auto mask0 = test(vector<T, N, Base, Env>::splat(N));
    auto index = andnot(vector<T, N, Base, Env>::splat(N));
    return mask0.select(index.swizzle(src0), index.swizzle(src1));
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::vmask_type vector<T, N, Base, Env>::swizzle(const vmask_type& src) {
    return std::bit_cast<vmask_type>(swizzle(std::bit_cast<vector>(src)));
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::swizzle(const Env::template vector<T, 16 / sizeof(T)>& src)
    requires(16 / sizeof(T) != N)
  {
    vector<T, N, Base, Env> result;
    result.raw[0] = raw[0].swizzle(src);
    result.raw[1] = raw[1].swizzle(src);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  template<usize shift_amount>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::shl() {
    vector<T, N, Base, Env> result;
    result.raw[0] = raw[0].template shl<shift_amount>();
    result.raw[1] = raw[1].template shl<shift_amount>();
    return result;
  }

  template<class T, usize N, class Base, class Env>
  template<usize shift_amount>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::shr() {
    vector<T, N, Base, Env> result;
    result.raw[0] = raw[0].template shr<shift_amount>();
    result.raw[1] = raw[1].template shr<shift_amount>();
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::andnot(const vector<T, N, Base, Env>& second) const {
    vector<T, N, Base, Env> result;
    result.raw[0] = raw[0].andnot(second.raw[0]);
    result.raw[1] = raw[1].andnot(second.raw[1]);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr T vector<T, N, Base, Env>::reduce_add() const {
    return (raw[0] + raw[1]).reduce_add();
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr T vector<T, N, Base, Env>::reduce_or() const {
    return (raw[0] | raw[1]).reduce_or();
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr T vector<T, N, Base, Env>::reduce_xor() const {
    return (raw[0] ^ raw[1]).reduce_xor();
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::zip_low(const vector<T, N, Base, Env>& second) const {
    vector<T, N, Base, Env> result;
    result.raw[0] = raw[0].zip_low(second.raw[0]);
    result.raw[1] = raw[0].zip_high(second.raw[0]);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::zip_high(const vector<T, N, Base, Env>& second) const {
    vector<T, N, Base, Env> result;
    result.raw[0] = raw[1].zip_low(second.raw[1]);
    result.raw[1] = raw[1].zip_high(second.raw[1]);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::zip_low_128lanes(const vector<T, N, Base, Env>& second) const {
    vector<T, N, Base, Env> result;
    result.raw[0] = raw[0].zip_low_128lanes(second.raw[0]);
    result.raw[1] = raw[1].zip_low_128lanes(second.raw[1]);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> vector<T, N, Base, Env>::zip_high_128lanes(const vector<T, N, Base, Env>& second) const {
    vector<T, N, Base, Env> result;
    result.raw[0] = raw[0].zip_high_128lanes(second.raw[0]);
    result.raw[1] = raw[1].zip_high_128lanes(second.raw[1]);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::vmask_type vector<T, N, Base, Env>::test_vm(const vector& second) const {
    vector<T, N, Base, Env>::mask_type result;
    result.raw[0] = raw[0].test_vm(second.raw[0]);
    result.raw[1] = raw[1].test_vm(second.raw[1]);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::bmask_type vector<T, N, Base, Env>::test_bm(const vector& second) const {
    bmask_type result = 0;
    result |= static_cast<bmask_type>(raw[0].test_bm(second.raw[0]));
    result |= static_cast<bmask_type>(raw[1].test_bm(second.raw[1])) << (N / 2);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::test(const vector& second) const {
    return test_vm(second);
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::vmask_type vector<T, N, Base, Env>::eq_vm(const vector& second) const {
    vector<T, N, Base, Env>::mask_type result;
    result.raw[0] = raw[0].eq_vm(second.raw[0]);
    result.raw[1] = raw[1].eq_vm(second.raw[1]);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::eq(const vector& second) const {
    return eq_vm(second);
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::vmask_type vector<T, N, Base, Env>::neq_vm(const vector& second) const {
    vector<T, N, Base, Env>::mask_type result;
    result.raw[0] = raw[0].neq_vm(second.raw[0]);
    result.raw[1] = raw[1].neq_vm(second.raw[1]);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::neq(const vector& second) const {
    return neq_vm(second);
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::vmask_type vector<T, N, Base, Env>::gt_vm(const vector& second) const {
    vector<T, N, Base, Env>::mask_type result;
    result.raw[0] = raw[0].gt_vm(second.raw[0]);
    result.raw[1] = raw[1].gt_vm(second.raw[1]);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::gt(const vector& other) const {
    return gt_vm(other);
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::vmask_type vector<T, N, Base, Env>::nonzeros_vm() const {
    vector<T, N, Base, Env>::mask_type result;
    result.raw[0] = raw[0].nonzeros_vm();
    result.raw[1] = raw[1].nonzeros_vm();
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::nonzeros() const {
    return nonzeros_vm();
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr usize vector<T, N, Base, Env>::nonzeros_count() const {
    usize result = 0;
    result += raw[0].nonzeros_count();
    result += raw[1].nonzeros_count();
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::vmask_type vector<T, N, Base, Env>::zeros_vm() const {
    vector<T, N, Base, Env>::mask_type result;
    result.raw[0] = raw[0].zeros_vm();
    result.raw[1] = raw[1].zeros_vm();
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::zeros() const {
    return zeros_vm();
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr usize vector<T, N, Base, Env>::zeros_count() const {
    usize result = 0;
    result += raw[0].zeros_count();
    result += raw[1].zeros_count();
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::vmask_type vector<T, N, Base, Env>::msb_vm() const {
    vector<T, N, Base, Env>::mask_type result;
    result.raw[0] = raw[0].msb_vm();
    result.raw[1] = raw[1].msb_vm();
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>::mask_type vector<T, N, Base, Env>::msb() const {
    return msb_vm();
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE std::array<T, N> vector<T, N, Base, Env>::to_array() const {
    return std::bit_cast<std::array<T, N>>(raw);
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr bool operator==(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first.raw == second.raw;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> operator~(const vector<T, N, Base, Env>& first) {
    vector<T, N, Base, Env> result;
    result.raw[0] = ~first.raw[0];
    result.raw[1] = ~first.raw[1];
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> operator&(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    vector<T, N, Base, Env> result;
    result.raw[0] = first.raw[0] & second.raw[0];
    result.raw[1] = first.raw[1] & second.raw[1];
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>& operator&=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first & second;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> operator|(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    vector<T, N, Base, Env> result;
    result.raw[0] = first.raw[0] | second.raw[0];
    result.raw[1] = first.raw[1] | second.raw[1];
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>& operator|=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first | second;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> operator^(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    vector<T, N, Base, Env> result;
    result.raw[0] = first.raw[0] ^ second.raw[0];
    result.raw[1] = first.raw[1] ^ second.raw[1];
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>& operator^=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first ^ second;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> operator+(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    vector<T, N, Base, Env> result;
    result.raw[0] = first.raw[0] + second.raw[0];
    result.raw[1] = first.raw[1] + second.raw[1];
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>& operator+=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first + second;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> operator-(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    vector<T, N, Base, Env> result;
    result.raw[0] = first.raw[0] - second.raw[0];
    result.raw[1] = first.raw[1] - second.raw[1];
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>& operator-=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first - second;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> operator*(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    vector<T, N, Base, Env> result;
    result.raw[0] = first.raw[0] * second.raw[0];
    result.raw[1] = first.raw[1] * second.raw[1];
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>& operator*=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first * second;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> operator<<(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    vector<T, N, Base, Env> result;
    result.raw[0] = first.raw[0] << second.raw[0];
    result.raw[1] = first.raw[1] << second.raw[1];
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>& operator<<=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first << second;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env> operator>>(const vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    vector<T, N, Base, Env> result;
    result.raw[0] = first.raw[0] >> second.raw[0];
    result.raw[1] = first.raw[1] >> second.raw[1];
    return result;
  }

  template<class T, usize N, class Base, class Env>
  LPS_INLINE constexpr vector<T, N, Base, Env>& operator>>=(vector<T, N, Base, Env>& first, const vector<T, N, Base, Env>& second) {
    return first = first >> second;
  }

}  // namespace lps::doubling
