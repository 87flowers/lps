#pragma once

#include "lps/generic/basic_vector_mask.hpp"
#include "lps/generic/vector.hpp"
#include "lps/stdint.hpp"

namespace lps {

  template<class T, usize N>
  using vector = generic::vector<T, N>;

  template<class T, usize N>
  using vector_mask = generic::vector_mask<T, N>;

}  // namespace lps

namespace lps::prelude {

  using u32x4 = vector<u32, 4>;
  using i32x4 = vector<i32, 4>;
  using vm32x4 = vector_mask<u32, 4>;
  using m32x4 = vm32x4;

}  // namespace lps::prelude
