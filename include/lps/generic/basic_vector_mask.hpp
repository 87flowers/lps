#pragma once

#include "lps/generic/basic_vector_mask.def.hpp"
#include "lps/generic/vector.def.hpp"
#include "lps/stdint.hpp"

#include <array>

namespace lps::generic {

  template<class T, usize N>
  constexpr basic_vector_mask<T, N> basic_vector_mask<T, N>::zero() {
    return vector { std::array<T, N> {} };
  }

  template<class T, usize N>
  constexpr basic_vector_mask<T, N> basic_vector_mask<T, N>::splat(bool value) {
    basic_vector_mask v;
    v.raw = inner_type::splat(value ? true_value : false_value);
    return v;
  }

  template<class T, usize N>
  constexpr void basic_vector_mask<T, N>::set(usize index, bool value) {
    raw.raw[index] = value ? true_value : false_value;
  }

  template<class T, usize N>
  template<class U>
    requires std::is_same_v<T, detail::mask_element_t<U>>
  constexpr vector<U, N> basic_vector_mask<T, N>::select(const vector<U, N>& v0, const vector<U, N>& v1) {
    vector<U, N> result;
    for (usize i = 0; i < N; i++) {
      result.raw[i] = raw.raw[i] == false_value ? v0.raw[i] : v1.raw[i];
    }
    return result;
  }

  template<class T, usize N>
  [[nodiscard]] std::array<T, N> basic_vector_mask<T, N>::to_array() const {
    return raw;
  }

}  // namespace lps::generic
