#pragma once

#include "lps/detail/bit_mask_base.hpp"
#include "lps/detail/mask_element.hpp"
#include "lps/generic/generic.fwd.hpp"
#include "lps/stdint.hpp"

#include <array>
#include <type_traits>

namespace lps::generic {

  template<class T, usize N>
  struct basic_vector_mask {
    static constexpr usize size = N;
    using inner_type = vector<T, N>;

    explicit constexpr basic_vector_mask() = default;

    static constexpr basic_vector_mask zero();
    static constexpr basic_vector_mask splat(bool value);

    constexpr void set(usize index, bool value);

    template<class U>
      requires std::is_same_v<T, detail::mask_element_t<U>>
    constexpr vector<U, N> mask(const vector<U, N>& v1);

    template<class U>
      requires std::is_same_v<T, detail::mask_element_t<U>>
    constexpr vector<U, N> select(const vector<U, N>& v0, const vector<U, N>& v1);

    template<class U>
      requires std::is_same_v<T, detail::mask_element_t<U>>
    constexpr vector<U, N> compress(const vector<U, N>& v);

    [[nodiscard]] std::array<T, N> to_array() const;
    [[nodiscard]] detail::bit_mask_base_t<N> to_bits() const;
    [[nodiscard]] inner_type to_vector() const;

    friend constexpr bool operator== <T, N>(const basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

    friend constexpr basic_vector_mask<T, N> andnot<T, N>(const basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

    friend constexpr basic_vector_mask<T, N> operator~ <T, N>(const basic_vector_mask<T, N>& first);

    friend constexpr basic_vector_mask<T, N> operator& <T, N>(const basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

    friend constexpr basic_vector_mask<T, N>& operator&= <T, N>(basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

    friend constexpr basic_vector_mask<T, N> operator| <T, N>(const basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);

    friend constexpr basic_vector_mask<T, N>& operator|= <T, N>(basic_vector_mask<T, N>& first, const basic_vector_mask<T, N>& second);
  private:
    static constexpr T false_value = T { 0 };
    static constexpr T true_value = static_cast<T>(~T { 0 });

    inner_type raw;
  };

}  // namespace lps::generic
