#pragma once

#include "lps/generic/generic.fwd.hpp"
#include "lps/stdint.hpp"

#include <array>

namespace lps::generic {

  template<class T, usize N>
  struct vector {
    using element_type = T;
    static constexpr usize size = N;
    using mask_type = vector_mask<T, N>;

    constexpr vector() = default;

    explicit constexpr vector(const std::array<T, N>& src) :
        raw(src) {
    }

    static constexpr vector zero();
    static constexpr vector splat(T value);
    static vector load(const void* src);

    constexpr T read(usize i) const;

    template<class U>
    constexpr vector<U, std::max(N, 16 / sizeof(U))> convert();

    template<class V, usize extract_index>
    constexpr V extract_aligned();

    // forall i: result[i] = src[this[i]]
    constexpr vector swizzle(const vector& src);
    // forall i: result[i] = concat[this[i]] where contact is concatenation of src0 and src1
    constexpr vector swizzle(const vector& src0, const vector& src1);

    constexpr mask_type swizzle(const mask_type& src);

    template<usize M>
      requires(M != N)
    constexpr vector swizzle(const vector<T, M>& src);

    template<usize shift_amount>
    constexpr vector shl();
    template<usize shift_amount>
    constexpr vector shr();

    constexpr vector andnot(const vector& second) const;

    constexpr T reduce_add() const;
    constexpr T reduce_or() const;
    constexpr T reduce_xor() const;

    constexpr vector zip_low(const vector& second) const;
    constexpr vector zip_high(const vector& second) const;

    constexpr vector zip_low_128lanes(const vector& second) const;
    constexpr vector zip_high_128lanes(const vector& second) const;

    constexpr mask_type test_vm(const vector& second) const;
    constexpr mask_type test(const vector& second) const;

    constexpr mask_type eq_vm(const vector& second) const;
    constexpr mask_type eq(const vector& second) const;

    constexpr mask_type neq_vm(const vector& second) const;
    constexpr mask_type neq(const vector& second) const;

    constexpr mask_type gt_vm(const vector& second) const;
    constexpr mask_type gt(const vector& second) const;

    constexpr mask_type nonzeros_vm() const;
    constexpr mask_type nonzeros() const;
    constexpr usize nonzeros_count() const;

    constexpr mask_type zeros_vm() const;
    constexpr mask_type zeros() const;
    constexpr usize zeros_count() const;

    constexpr mask_type msb_vm() const;
    constexpr mask_type msb() const;

    [[nodiscard]] std::array<T, N> to_array() const;

    std::array<T, N> raw;
  };

  template<class T, usize N>
  constexpr bool operator==(const vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N> operator~(const vector<T, N>& first);

  template<class T, usize N>
  constexpr vector<T, N> operator&(const vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N>& operator&=(vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N> operator|(const vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N>& operator|=(vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N> operator^(const vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N>& operator^=(vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N> operator+(const vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N>& operator+=(vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N> operator-(const vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N>& operator-=(vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N> operator*(const vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N>& operator*=(vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N> operator<<(const vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N>& operator<<=(vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N> operator>>(const vector<T, N>& first, const vector<T, N>& second);

  template<class T, usize N>
  constexpr vector<T, N>& operator>>=(vector<T, N>& first, const vector<T, N>& second);

}  // namespace lps::generic
