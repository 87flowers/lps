#pragma once

#include "lps/generic/generic.fwd.hpp"
#include "lps/stdint.hpp"

#include <array>
#include <type_traits>

namespace lps::generic {

  template<class T, usize N>
  struct basic_vector_mask {
    static constexpr usize size = N;

    explicit constexpr basic_vector_mask() = default;

    static constexpr basic_vector_mask zero();
    static constexpr basic_vector_mask splat(bool value);

    constexpr void set(usize index, bool value);

    template<class U, class = std::enable_if_t<std::is_same_v<T, detail::mask_element_t<U>>>>
    constexpr vector<U, N> select(const vector<U, N>& v0, const vector<U, N>& v1);

    [[nodiscard]] std::array<T, N> to_array() const;
  private:
    static constexpr T false_value = T { 0 };
    static constexpr T true_value = ~T { 0 };

    using inner_type = vector<T, N>;
    inner_type raw;
  };

}  // namespace lps::generic
