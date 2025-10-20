#pragma once

#include "lps/doubling/basic_vector_mask.def.hpp"
#include "lps/doubling/vector.def.hpp"
#include "lps/generic/basic_vector_mask.hpp"
#include "lps/generic/vector.hpp"
#include "lps/stdint.hpp"

#include <array>

namespace lps::doubling {

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env>::basic_vector_mask(detail::bit_mask_base_t<N> value) {
    raw[0] = Base { static_cast<detail::bit_mask_base_t<N / 2>>(value) };
    raw[1] = Base { static_cast<detail::bit_mask_base_t<N / 2>>(value >> (N / 2)) };
  }

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env> basic_vector_mask<T, N, Base, Env>::zero() {
    basic_vector_mask<T, N, Base, Env> result;
    result.raw[0] = Base::zero();
    result.raw[1] = Base::zero();
    return result;
  }

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env> basic_vector_mask<T, N, Base, Env>::splat(bool value) {
    basic_vector_mask<T, N, Base, Env> result;
    result.raw[0] = Base::splat(value);
    result.raw[1] = Base::splat(value);
    return result;
  }

  template<class T, usize N, class Base, class Env>
  constexpr void basic_vector_mask<T, N, Base, Env>::set(usize index, bool value) {
    raw[index / (N / 2)].set(index % (N / 2), value);
  }

  template<class T, usize N, class Base, class Env>
  template<class V>
    requires std::is_same_v<V, typename Env::template vector<typename V::element_type, N>>
  constexpr V basic_vector_mask<T, N, Base, Env>::mask(const V& v1) {
    return select(V::zero(), v1);
  }

  template<class T, usize N, class Base, class Env>
  template<class V>
    requires std::is_same_v<V, typename Env::template vector<typename V::element_type, N>>
  constexpr V basic_vector_mask<T, N, Base, Env>::select(const V& v0, const V& v1) {
    return std::bit_cast<V>(std::bit_cast<generic::basic_vector_mask<T, N>>(*this).select(
      std::bit_cast<generic::vector<typename V::element_type, N>>(v0), std::bit_cast<generic::vector<typename V::element_type, N>>(v1)));
  }

  template<class T, usize N, class Base, class Env>
  template<class V>
    requires std::is_same_v<V, typename Env::template vector<typename V::element_type, N>>
  constexpr V basic_vector_mask<T, N, Base, Env>::compress(const V& v) {
    return std::bit_cast<V>(
      std::bit_cast<generic::basic_vector_mask<T, N>>(*this).compress(std::bit_cast<generic::vector<typename V::element_type, N>>(v)));
  }

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env> basic_vector_mask<T, N, Base, Env>::andnot(const basic_vector_mask<T, N, Base, Env>& second) const {
    return std::bit_cast<basic_vector_mask<T, N, Base, Env>>(
      std::bit_cast<generic::basic_vector_mask<T, N>>(*this).andnot(std::bit_cast<generic::basic_vector_mask<T, N>>(second)));
  }

  template<class T, usize N, class Base, class Env>
  [[nodiscard]] usize basic_vector_mask<T, N, Base, Env>::popcount() const {
    return std::bit_cast<generic::basic_vector_mask<T, N>>(*this).popcount();
  }

  template<class T, usize N, class Base, class Env>
  [[nodiscard]] std::array<T, N> basic_vector_mask<T, N, Base, Env>::to_array() const {
    return std::bit_cast<std::array<T, N>>(*this);
  }

  template<class T, usize N, class Base, class Env>
  [[nodiscard]] detail::bit_mask_base_t<N> basic_vector_mask<T, N, Base, Env>::to_bits() const {
    return std::bit_cast<generic::basic_vector_mask<T, N>>(*this).to_bits();
  }

  template<class T, usize N, class Base, class Env>
  [[nodiscard]] basic_vector_mask<T, N, Base, Env>::inner_type basic_vector_mask<T, N, Base, Env>::to_vector() const {
    return std::bit_cast<basic_vector_mask<T, N, Base, Env>::inner_type>(*this);
  }

  template<class T, usize N, class Base, class Env>
  constexpr bool operator==(const basic_vector_mask<T, N, Base, Env>& first, const basic_vector_mask<T, N, Base, Env>& second) {
    return first.raw == second.raw;
  }

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env> operator~(const basic_vector_mask<T, N, Base, Env>& first) {
    return std::bit_cast<basic_vector_mask<T, N, Base, Env>>(~std::bit_cast<generic::basic_vector_mask<T, N>>(first));
  }

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env> operator&(const basic_vector_mask<T, N, Base, Env>& first,
                                                         const basic_vector_mask<T, N, Base, Env>& second) {
    return std::bit_cast<basic_vector_mask<T, N, Base, Env>>(std::bit_cast<generic::basic_vector_mask<T, N>>(first) &
                                                             std::bit_cast<generic::basic_vector_mask<T, N>>(second));
  }

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env>& operator&=(basic_vector_mask<T, N, Base, Env>& first,
                                                           const basic_vector_mask<T, N, Base, Env>& second) {
    return first = first & second;
  }

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env> operator|(const basic_vector_mask<T, N, Base, Env>& first,
                                                         const basic_vector_mask<T, N, Base, Env>& second) {
    return std::bit_cast<basic_vector_mask<T, N, Base, Env>>(std::bit_cast<generic::basic_vector_mask<T, N>>(first) |
                                                             std::bit_cast<generic::basic_vector_mask<T, N>>(second));
  }

  template<class T, usize N, class Base, class Env>
  constexpr basic_vector_mask<T, N, Base, Env>& operator|=(basic_vector_mask<T, N, Base, Env>& first,
                                                           const basic_vector_mask<T, N, Base, Env>& second) {
    return first = first | second;
  }

}  // namespace lps::doubling
