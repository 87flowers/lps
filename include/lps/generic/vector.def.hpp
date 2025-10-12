#pragma once

#include "lps/generic/generic.fwd.hpp"
#include "lps/stdint.hpp"

#include <array>

namespace lps::generic {

  template<class T, usize N>
  struct vector {
    std::array<T, N> raw;

    static constexpr usize size = N;

    explicit constexpr vector() = default;

    explicit constexpr vector(const std::array<T, N>& src) :
        raw(src) {
    }

    static constexpr vector zero();
    static constexpr vector splat(T value);
    static vector load(const void* src);

    [[nodiscard]] std::array<T, N> to_array() const;
  };

}  // namespace lps::generic
