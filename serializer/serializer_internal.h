#ifndef __SERIALIZER_INTERNAL_H__
#define __SERIALIZER_INTERNAL_H__

#include <string_view>
#include <tuple>

#include "member_property.h"

namespace chaolib {
namespace serializer {

template <typename Class, typename T, typename... ALIAS>
constexpr auto make_member_property(T Class::*member, ALIAS... aliases)
    -> MemberProperty<Class, T, std::tuple_size_v<std::tuple<ALIAS...>>> {
  using Tuple = std::tuple<ALIAS...>;
  constexpr auto size = std::tuple_size_v<Tuple>;
  return MemberProperty<Class, T, size>(
      member,
      std::array<std::string_view, size>{aliases...});
}

template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
  (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
}

} // namespace serializer
} // namespace chaolib

#endif // __SERIALIZER_INTERNAL_H__
