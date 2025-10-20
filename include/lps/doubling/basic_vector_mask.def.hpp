#pragma once

#include "lps/detail/bit_mask_base.hpp"
#include "lps/detail/mask_element.hpp"
#include "lps/doubling/doubling.fwd.hpp"
#include "lps/stdint.hpp"

#include <array>
#include <type_traits>

namespace lps::doubling {

  template<class T, usize N, class Base, class Env>
  struct basic_vector_mask {
    static constexpr usize size = N;
    using inner_type = Env::template vector<T, N>;

    constexpr basic_vector_mask() = default;
    explicit constexpr basic_vector_mask(detail::bit_mask_base_t<N> value);

    static constexpr basic_vector_mask zero();
    static constexpr basic_vector_mask splat(bool value);

    constexpr void set(usize index, bool value);

    template<class V>
      requires std::is_same_v<V, typename Env::template vector<typename V::element_type, N>>
    constexpr V mask(const V& v1);

    template<class V>
      requires std::is_same_v<V, typename Env::template vector<typename V::element_type, N>>
    constexpr V select(const V& v0, const V& v1);

    template<class V>
      requires std::is_same_v<V, typename Env::template vector<typename V::element_type, N>>
    constexpr V compress(const V& v);

    constexpr basic_vector_mask andnot(const basic_vector_mask& second) const;

    [[nodiscard]] usize popcount() const;

    [[nodiscard]] std::array<T, N> to_array() const;
    [[nodiscard]] detail::bit_mask_base_t<N> to_bits() const;
    [[nodiscard]] inner_type to_vector() const;

    static constexpr T false_value = T { 0 };
    static constexpr T true_value = static_cast<T>(~T { 0 });

    std::array<Base, 2> raw;
  };

  template<class T, usize N, class Base, class Env>
  constexpr bool operator==(const basic_vector_mask<T, N, Base, Env>& first, const basic_vector_mask<T, N, Base, Env>& second);

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env> operator~(const basic_vector_mask<T, N, Base, Env>& first);

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env> operator&(const basic_vector_mask<T, N, Base, Env>& first,
                                                         const basic_vector_mask<T, N, Base, Env>& second);

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env>& operator&=(basic_vector_mask<T, N, Base, Env>& first,
                                                           const basic_vector_mask<T, N, Base, Env>& second);

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env> operator|(const basic_vector_mask<T, N, Base, Env>& first,
                                                         const basic_vector_mask<T, N, Base, Env>& second);

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env>& operator|=(basic_vector_mask<T, N, Base, Env>& first,
                                                           const basic_vector_mask<T, N, Base, Env>& second);

}  // namespace lps::doubling
