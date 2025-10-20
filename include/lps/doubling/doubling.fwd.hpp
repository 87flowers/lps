#pragma once

#include "lps/detail/mask_element.hpp"
#include "lps/stdint.hpp"

namespace lps::doubling {

  template<class T, usize N, class Base, class Env>
  struct vector;

  template<class T, usize N, class Base, class Env>
  struct basic_vector_mask;

  template<class T, usize N, class Base, class Env>
  using vector_mask = basic_vector_mask<detail::mask_element_t<T>, N, Base, Env>;

}  // namespace lps::doubling
