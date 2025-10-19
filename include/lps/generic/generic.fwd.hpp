#pragma once

#include "lps/detail/mask_element.hpp"
#include "lps/stdint.hpp"

namespace lps::generic {

  template<class T, usize N>
  struct vector;

  template<class T, usize N>
  struct basic_vector_mask;

  template<class T, usize N>
  using vector_mask = basic_vector_mask<detail::mask_element_t<T>, N>;

  template<class T, usize N>
  constexpr bool operator==(const basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

  template<class T, usize N>
  constexpr basic_vector_mask<T, N> andnot(const basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

  template<class T, usize N>
  constexpr basic_vector_mask<T, N> operator~(const basic_vector_mask<T, N>& first);

  template<class T, usize N>
  constexpr basic_vector_mask<T, N> operator&(const basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

  template<class T, usize N>
  constexpr basic_vector_mask<T, N>& operator&=(basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

  template<class T, usize N>
  constexpr basic_vector_mask<T, N> operator|(const basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

  template<class T, usize N>
  constexpr basic_vector_mask<T, N>& operator|=(basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

}  // namespace lps::generic
