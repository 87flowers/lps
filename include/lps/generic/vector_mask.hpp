#pragma once

#include "lps/generic/vector.def.hpp"
#include "lps/generic/vector_mask.def.hpp"
#include "lps/stdint.hpp"

#include <array>

namespace lps::generic {

  template<class T, usize N>
  constexpr vector_mask<T, N> vector_mask<T, N>::zero() {
    return vector { std::array<T, N> {} };
  }

  template<class T, usize N>
  constexpr vector_mask<T, N> vector_mask<T, N>::splat(bool value) {
    vector_mask v;
    v.raw = inner_type::splat(value ? element_info::true_value : element_info::false_value);
    return v;
  }

  template<class T, usize N>
  [[nodiscard]] std::array<T, N> vector_mask<T, N>::to_array() const {
    return raw;
  }

}  // namespace lps::generic
