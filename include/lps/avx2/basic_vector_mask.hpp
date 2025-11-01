#pragma once

#include "lps/avx2/basic_vector_mask.def.hpp"
#include "lps/avx2/vector.def.hpp"
#include "lps/generic/basic_vector_mask.def.hpp"
#include "lps/generic/vector.def.hpp"
#include "lps/stdint.hpp"

#include <array>
#include <immintrin.h>
#include <bit>

namespace lps::avx2 {

  template<class T, usize N, class Env>
  LPS_INLINE constexpr basic_vector_mask<T, N, Env>::basic_vector_mask(inner_type::raw_type value) :
      raw(value) {
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr basic_vector_mask<T, N, Env>::basic_vector_mask(detail::bit_mask_base_t<N> value) {
    std::array<T, N> a;
    for (usize i = 0; i < N; i++) {
      a[i] = ((value >> i) & 1) ? true_value : false_value;
    }
    raw = std::bit_cast<inner_type>(a);
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr basic_vector_mask<T, N, Env> basic_vector_mask<T, N, Env>::zero() {
    basic_vector_mask<T, N, Env> result;
    result.raw = inner_type::zero();
    return result;
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr basic_vector_mask<T, N, Env> basic_vector_mask<T, N, Env>::splat(bool value) {
    basic_vector_mask<T, N, Env> result;
    result.raw = inner_type::splat(value ? true_value : false_value);
    return result;
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr void basic_vector_mask<T, N, Env>::set(usize index, bool value) {
    std::array<T, N> a = to_array();
    a[index] = value ? true_value : false_value;
    raw = std::bit_cast<inner_type>(a);
  }

  template<class T, usize N, class Env>
  template<class V>
    requires std::is_same_v<V, typename Env::template vector<typename V::element_type, N>>
  LPS_INLINE constexpr V basic_vector_mask<T, N, Env>::mask(const V& v1) const {
    return V { raw.raw } & v1;
  }

  template<class T, usize N, class Env>
  template<class V>
    requires std::is_same_v<V, typename Env::template vector<typename V::element_type, N>>
  LPS_INLINE constexpr V basic_vector_mask<T, N, Env>::select(const V& v0, const V& v1) const {
    if constexpr (V::is_128_bit) {
      return V { _mm_blendv_epi8(v0.raw, v1.raw, raw.raw) };
    } else {
      return V { _mm256_blendv_epi8(v0.raw, v1.raw, raw.raw) };
    }
  }

  template<class T, usize N, class Env>
  template<class V>
    requires std::is_same_v<V, typename Env::template vector<typename V::element_type, N>>
  LPS_INLINE constexpr V basic_vector_mask<T, N, Env>::compress(const V& v) const {
  #if defined(__BMI2__) && defined(__AVX2__)
    using elem_t = typename V::element_type;
    constexpr size_t elem_bytes = sizeof(elem_t);

    if constexpr (elem_bytes == 1) {
      return std::bit_cast<V>(
        std::bit_cast<generic::basic_vector_mask<T, N>>(*this).compress(
          std::bit_cast<generic::vector<typename V::element_type, N>>(v)));
    } else {
      constexpr unsigned elems_per_128 = 16u / (unsigned)elem_bytes;
      static_assert(elems_per_128 >= 1 && elems_per_128 <= 16,
                    "unsupported element size for this compress implementation");

      constexpr uint32_t make_elem_msb_mask() {
        uint32_t m = 0;
        for (unsigned i = 0; i < elems_per_128; ++i) {
          unsigned bitpos = i * (unsigned)elem_bytes + (unsigned)elem_bytes - 1;
          m |= (1u << bitpos);
        }
        return m;
      }
      constexpr uint32_t elem_msb_mask = make_elem_msb_mask();

      if constexpr (V::is_128_bit) {
        __m128i maskv = this->raw.raw;
        __m128i srcv  = v.raw;

        int mov = _mm_movemask_epi8(maskv);
        uint32_t elem_mask = _pext_u32((uint32_t)mov, elem_msb_mask);

        alignas(16) uint8_t src_bytes[16];
        alignas(16) uint8_t out_bytes[16];
        _mm_storeu_si128((__m128i*)src_bytes, srcv);

        size_t write = 0;
        for (unsigned e = 0; e < (unsigned)N; ++e) {
          if ( (elem_mask >> e) & 1u ) {

            const uint8_t* srcp = src_bytes + e * elem_bytes;
            std::memcpy(out_bytes + write, srcp, elem_bytes);
            write += elem_bytes;
          }
        }
        if (write < sizeof(out_bytes)) std::memset(out_bytes + write, 0, sizeof(out_bytes) - write);

        __m128i res = _mm_loadu_si128((__m128i*)out_bytes);
        return V{res};
      } else {
        __m256i maskv256 = this->raw.raw;
        __m256i srcv256  = v.raw;

        __m128i mask_lo = _mm256_castsi256_si128(maskv256);
        __m128i mask_hi = _mm256_extracti128_si256(maskv256, 1);
        int mov_lo = _mm_movemask_epi8(mask_lo);
        int mov_hi = _mm_movemask_epi8(mask_hi);

        uint32_t elem_mask_lo = _pext_u32((uint32_t)mov_lo, elem_msb_mask);
        uint32_t elem_mask_hi = _pext_u32((uint32_t)mov_hi, elem_msb_mask);

        uint64_t elem_mask = uint64_t(elem_mask_lo) | (uint64_t(elem_mask_hi) << elems_per_128);

        alignas(32) uint8_t src_bytes[32];
        alignas(32) uint8_t out_bytes[32];
        _mm256_storeu_si256((__m256i*)src_bytes, srcv256);

        size_t write = 0;
        for (unsigned e = 0; e < (unsigned)N; ++e) {
          if ( (elem_mask >> e) & 1u ) {
            const uint8_t* srcp = src_bytes + e * elem_bytes;
            std::memcpy(out_bytes + write, srcp, elem_bytes);
            write += elem_bytes;
          }
        }
        if (write < sizeof(out_bytes)) std::memset(out_bytes + write, 0, sizeof(out_bytes) - write);

        __m256i res = _mm256_loadu_si256((__m256i*)out_bytes);
        return V{res};
      }
    }
  #else
    return std::bit_cast<V>(
      std::bit_cast<generic::basic_vector_mask<T, N>>(*this).compress(std::bit_cast<generic::vector<typename V::element_type, N>>(v)));
  #endif
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr basic_vector_mask<T, N, Env> basic_vector_mask<T, N, Env>::andnot(const basic_vector_mask<T, N, Env>& second) const {
    if constexpr (inner_type::is_128_bit) {
      return basic_vector_mask { _mm_andnot_si128(second.raw.raw, raw.raw) };
    } else {
      return basic_vector_mask { _mm256_andnot_si256(second.raw.raw, raw.raw) };
    }
  }

  template<class T, usize N, class Env>
  [[nodiscard]] LPS_INLINE usize basic_vector_mask<T, N, Env>::popcount() const {
    if constexpr (inner_type::is_128_bit) {
      if constexpr (sizeof(T) == sizeof(u8)) {
        return static_cast<usize>(std::popcount(static_cast<u16>(_mm_movemask_epi8(raw.raw))));
      } else if constexpr (sizeof(T) == sizeof(u16)) {
        return static_cast<usize>(std::popcount(static_cast<u16>(_mm_movemask_epi8(raw.raw) & 0xAAAA)));
      } else if constexpr (sizeof(T) == sizeof(u32)) {
        return static_cast<usize>(std::popcount(static_cast<u8>(_mm_movemask_ps((__m128)raw.raw))));
      } else if constexpr (sizeof(T) == sizeof(u64)) {
        return static_cast<usize>(std::popcount(static_cast<u8>(_mm_movemask_pd((__m128d)raw.raw))));
      } else {
        static_assert(false);
      }
    } else {
      if constexpr (sizeof(T) == sizeof(u8)) {
        return static_cast<usize>(std::popcount(static_cast<u32>(_mm256_movemask_epi8(raw.raw))));
      } else if constexpr (sizeof(T) == sizeof(u16)) {
        return static_cast<usize>(std::popcount(static_cast<u32>(_mm256_movemask_epi8(raw.raw)) & 0xAAAAAAAA));
      } else if constexpr (sizeof(T) == sizeof(u32)) {
        return static_cast<usize>(std::popcount(static_cast<u8>(_mm256_movemask_ps((__m256)raw.raw))));
      } else if constexpr (sizeof(T) == sizeof(u64)) {
        return static_cast<usize>(std::popcount(static_cast<u8>(_mm256_movemask_pd((__m256d)raw.raw))));
      } else {
        static_assert(false);
      }
    }
  }

  template<class T, usize N, class Env>
  [[nodiscard]] LPS_INLINE std::array<T, N> basic_vector_mask<T, N, Env>::to_array() const {
    return raw.to_array();
  }

  template<class T, usize N, class Env>
  [[nodiscard]] LPS_INLINE detail::bit_mask_base_t<N> basic_vector_mask<T, N, Env>::to_bits() const {
    if constexpr (inner_type::is_128_bit) {
      if constexpr (sizeof(T) == sizeof(u8)) {
        return static_cast<u16>(_mm_movemask_epi8(raw.raw));
      } else if constexpr (sizeof(T) == sizeof(u16)) {
        return static_cast<u8>(_pext_u32(static_cast<u32>(_mm_movemask_epi8(raw.raw)), 0xAAAA));
      } else if constexpr (sizeof(T) == sizeof(u32)) {
        return static_cast<u8>(_mm_movemask_ps((__m128)raw.raw));
      } else if constexpr (sizeof(T) == sizeof(u64)) {
        return static_cast<u8>(_mm_movemask_pd((__m128d)raw.raw));
      } else {
        static_assert(false);
      }
    } else {
      if constexpr (sizeof(T) == sizeof(u8)) {
        return static_cast<u32>(_mm256_movemask_epi8(raw.raw));
      } else if constexpr (sizeof(T) == sizeof(u16)) {
        return static_cast<u16>(_pext_u32(static_cast<u32>(_mm256_movemask_epi8(raw.raw)), 0xAAAAAAAA));
      } else if constexpr (sizeof(T) == sizeof(u32)) {
        return static_cast<u8>(_mm256_movemask_ps((__m256)raw.raw));
      } else if constexpr (sizeof(T) == sizeof(u64)) {
        return static_cast<u8>(_mm256_movemask_pd((__m256d)raw.raw));
      } else {
        static_assert(false);
      }
    }
  }

  template<class T, usize N, class Env>
  [[nodiscard]] LPS_INLINE basic_vector_mask<T, N, Env>::inner_type basic_vector_mask<T, N, Env>::to_vector() const {
    return raw;
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr bool operator==(const basic_vector_mask<T, N, Env>& first, const basic_vector_mask<T, N, Env>& second) {
    return first.raw == second.raw;
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr basic_vector_mask<T, N, Env> operator~(const basic_vector_mask<T, N, Env>& first) {
    basic_vector_mask<T, N, Env> result;
    result.raw = ~first.raw;
    return result;
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr basic_vector_mask<T, N, Env> operator&(const basic_vector_mask<T, N, Env>& first, const basic_vector_mask<T, N, Env>& second) {
    basic_vector_mask<T, N, Env> result;
    result.raw = first.raw & second.raw;
    return result;
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr basic_vector_mask<T, N, Env>& operator&=(basic_vector_mask<T, N, Env>& first, const basic_vector_mask<T, N, Env>& second) {
    return first = first & second;
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr basic_vector_mask<T, N, Env> operator|(const basic_vector_mask<T, N, Env>& first, const basic_vector_mask<T, N, Env>& second) {
    basic_vector_mask<T, N, Env> result;
    result.raw = first.raw | second.raw;
    return result;
  }

  template<class T, usize N, class Env>
  LPS_INLINE constexpr basic_vector_mask<T, N, Env>& operator|=(basic_vector_mask<T, N, Env>& first, const basic_vector_mask<T, N, Env>& second) {
    return first = first | second;
  }

}  // namespace lps::avx2
