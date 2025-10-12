#pragma once

#include "lps/generic/generic.fwd.hpp"
#include "lps/stdint.hpp"

#include <array>

namespace lps::generic {

  template<class T, usize N>
  struct vector {
    static constexpr usize size = N;

    explicit constexpr vector() = default;

    explicit constexpr vector(const std::array<T, N>& src) :
        raw(src) {
    }

    static constexpr vector zero();
    static constexpr vector splat(T value);
    static vector load(const void* src);

    constexpr vector_mask<T, N> gt_vm(const vector& other) const;
    constexpr vector_mask<T, N> gt(const vector& other) const;

    [[nodiscard]] std::array<T, N> to_array() const;

    std::array<T, N> raw;
  };

  template<typename T, usize N>
  constexpr vector<T, N> operator+(const vector<T, N>& first, const vector<T, N>& second);

  template<typename T, usize N>
  constexpr vector<T, N>& operator+=(vector<T, N>& first, const vector<T, N>& second);

}  // namespace lps::generic
