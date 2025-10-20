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

  using u8x16 = vector<u8, 16>;
  using i8x16 = vector<i8, 16>;
  using vm8x16 = vector_mask<u8, 16>;
  using m8x16 = vm8x16;

  using u8x32 = vector<u8, 32>;
  using i8x32 = vector<i8, 32>;
  using vm8x32 = vector_mask<u8, 32>;
  using m8x32 = vm8x32;

  using u8x64 = vector<u8, 64>;
  using i8x64 = vector<i8, 64>;
  using vm8x64 = vector_mask<u8, 64>;
  using m8x64 = vm8x64;

  using u8x128 = vector<u8, 128>;
  using i8x128 = vector<i8, 128>;
  using vm8x128 = vector_mask<u8, 128>;
  using m8x128 = vm8x128;

  using u16x8 = vector<u16, 8>;
  using i16x8 = vector<i16, 8>;
  using vm16x8 = vector_mask<u16, 8>;
  using m16x8 = vm16x8;

  using u16x16 = vector<u16, 16>;
  using i16x16 = vector<i16, 16>;
  using vm16x16 = vector_mask<u16, 16>;
  using m16x16 = vm16x16;

  using u16x32 = vector<u16, 32>;
  using i16x32 = vector<i16, 32>;
  using vm16x32 = vector_mask<u16, 32>;
  using m16x32 = vm16x32;

  using u16x64 = vector<u16, 64>;
  using i16x64 = vector<i16, 64>;
  using vm16x64 = vector_mask<u16, 64>;
  using m16x64 = vm16x64;

  using u32x4 = vector<u32, 4>;
  using i32x4 = vector<i32, 4>;
  using vm32x4 = vector_mask<u32, 4>;
  using m32x4 = vm32x4;

  using u64x8 = vector<u64, 8>;
  using i64x8 = vector<i64, 8>;
  using vm64x8 = vector_mask<u64, 8>;
  using m64x8 = vm64x8;

}  // namespace lps::prelude
