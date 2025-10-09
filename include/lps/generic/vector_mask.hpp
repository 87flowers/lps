#pragma once

#include <array>

#include "lps/stdint.hpp"

namespace lps::generic {

template <class T, usize N> struct vector;

namespace detail {

template <class T> struct vector_mask_element_info;

template <> struct vector_mask_element_info<i8> {
  using type = u8;
  static constexpr u8 false_value = 0;
  static constexpr u8 true_value = 0xFF;
};

template <> struct vector_mask_element_info<i16> {
  using type = u16;
  static constexpr u16 false_value = 0;
  static constexpr u16 true_value = 0xFFFF;
};

template <> struct vector_mask_element_info<i32> {
  using type = u32;
  static constexpr u32 false_value = 0;
  static constexpr u32 true_value = 0xFFFFFFFF;
};

template <> struct vector_mask_element_info<i64> {
  using type = u64;
  static constexpr u64 false_value = 0;
  static constexpr u64 true_value = 0xFFFFFFFFFFFFFFFF;
};

template <> struct vector_mask_element_info<u8> {
  using type = u8;
  static constexpr u8 false_value = 0;
  static constexpr u8 true_value = 0xFF;
};

template <> struct vector_mask_element_info<u16> {
  using type = u16;
  static constexpr u16 false_value = 0;
  static constexpr u16 true_value = 0xFFFF;
};

template <> struct vector_mask_element_info<u32> {
  using type = u32;
  static constexpr u32 false_value = 0;
  static constexpr u32 true_value = 0xFFFFFFFF;
};

template <> struct vector_mask_element_info<u64> {
  using type = u64;
  static constexpr u64 false_value = 0;
  static constexpr u64 true_value = 0xFFFFFFFFFFFFFFFF;
};

template <> struct vector_mask_element_info<float> {
  static_assert(sizeof(u32) == sizeof(float));
  using type = u32;
  static constexpr u32 false_value = 0;
  static constexpr u32 true_value = 0xFFFFFFFF;
};

template <> struct vector_mask_element_info<double> {
  static_assert(sizeof(u64) == sizeof(double));
  using type = u64;
  static constexpr u64 false_value = 0;
  static constexpr u64 true_value = 0xFFFFFFFFFFFFFFFF;
};

} // namespace detail

template <class T, usize N> struct vector_mask {
  static constexpr usize size = N;
  using element_info = typename detail::vector_mask_element_info<T>;

  explicit constexpr vector_mask() = default;

  static constexpr vector_mask zero() { return vector{std::array<T, N>{}}; }
  static constexpr vector_mask splat(bool value) {
    vector_mask v;
    v.raw = inner_type::splat(value ? element_info::true_value
                                    : element_info::false_value);
    return v;
  }

  [[nodiscard]] std::array<T, N> to_array() const { return raw; }

private:
  using inner_type = vector<typename element_info::type, N>;
  inner_type raw;
};

} // namespace lps::generic
