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

  using u8x64 = vector<u8, 64>;
  using i8x64 = vector<i8, 64>;
  using vm8x64 = vector_mask<u8, 64>;
  using m8x64 = vm8x64;

  using u64x8 = vector<u64, 8>;
  using i64x8 = vector<i64, 8>;
  using vm64x8 = vector_mask<u64, 8>;
  using m64x8 = vm64x8;

  using u32x4 = vector<u32, 4>;
  using i32x4 = vector<i32, 4>;
  using vm32x4 = vector_mask<u32, 4>;
  using m32x4 = vm32x4;

}  // namespace lps::prelude
