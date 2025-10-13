#pragma once

#include "lps/generic/generic.fwd.hpp"
#include "lps/stdint.hpp"

#include <array>

namespace lps::generic {

  template<class T, usize N>
  struct vector {
    static constexpr usize size = N;

    explicit constexpr vector() = default;

    explicit constexpr vector(const std::array<T, N>& src) :
        raw(src) {
    }

    static constexpr vector zero();
    static constexpr vector splat(T value);
    static vector load(const void* src);

    template<class V, usize extract_index>
    constexpr V extract_aligned();

    // forall i: result[i] = src[this[i]]
    constexpr vector swizzle(const vector& src);
    // forall i: result[i] = concat[this[i]] where contact is concatenation of src0 and src1
    constexpr vector swizzle(const vector& src0, const vector& src1);

    template<usize shift_amount>
    constexpr vector shl();
    template<usize shift_amount>
    constexpr vector shr();

    constexpr vector zip_low(const vector& second) const;
    constexpr vector zip_high(const vector& second) const;

    constexpr vector_mask<T, N> test_vm(const vector& second) const;
    constexpr vector_mask<T, N> test(const vector& second) const;

    constexpr vector_mask<T, N> eq_vm(const vector& second) const;
    constexpr vector_mask<T, N> eq(const vector& second) const;

    constexpr vector_mask<T, N> neq_vm(const vector& second) const;
    constexpr vector_mask<T, N> neq(const vector& second) const;

    constexpr vector_mask<T, N> gt_vm(const vector& second) const;
    constexpr vector_mask<T, N> gt(const vector& second) const;

    constexpr vector_mask<T, N> nonzero_vm() const;
    constexpr vector_mask<T, N> nonzero() const;
    constexpr usize nonzero_count() const;

    constexpr vector_mask<T, N> msb_vm() const;
    constexpr vector_mask<T, N> msb() const;

    [[nodiscard]] std::array<T, N> to_array() const;

    std::array<T, N> raw;
  };

  template<typename T, usize N>
  constexpr bool operator==(const vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N> andnot(const vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N> operator~(const vector<T, N>& first);

  template<typename T, usize N>
  constexpr vector<T, N> operator&(const vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N>& operator&=(vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N> operator|(const vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N>& operator|=(vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N> operator^(const vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N>& operator^=(vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N> operator+(const vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N>& operator+=(vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N> operator-(const vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N>& operator-=(vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N> operator-(const vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N>& operator-=(vector<T, N>& first, const vector<T, N>& second);

}  // namespace lps::generic
