#pragma once

#include "lps/doubling/basic_vector_mask.hpp"
#include "lps/doubling/doubling.fwd.hpp"
#include "lps/doubling/vector.hpp"
#include "lps/sse4_2/basic_vector_mask.hpp"
#include "lps/sse4_2/sse4_2.fwd.hpp"
#include "lps/sse4_2/vector.hpp"
#include "lps/stdint.hpp"

#include <type_traits>

namespace lps::sse4_2 {

  struct environment;

  namespace detail {
    template<class T, usize N>
    struct vector_helper {
      using type = std::conditional_t<(sizeof(T) * N <= 16 * sizeof(u8)), sse4_2::vector<T, N, environment>,
                                      doubling::vector<T, N, typename vector_helper<T, N / 2>::type, environment>>;
    };

    template<class T>
    struct vector_helper<T, 0> {
      using type = void;
    };

    template<class T, usize N>
    struct vector_mask_helper {
      using type = std::conditional_t<(sizeof(T) * N <= 16 * sizeof(u8)), sse4_2::vector_mask<T, N, environment>,
                                      doubling::vector_mask<T, N, typename vector_mask_helper<T, N / 2>::type, environment>>;
    };

    template<class T>
    struct vector_mask_helper<T, 0> {
      using type = void;
    };
  }  // namespace detail

  struct environment {
    template<class T, usize N>
    using vector = typename detail::vector_helper<T, N>::type;

    template<class T, usize N>
    using vector_mask = typename detail::vector_mask_helper<T, N>::type;

    template<class T, usize N>
    using mask = typename detail::vector_mask_helper<T, N>::type;
  };

}  // namespace lps::sse4_2
