#ifndef __SERIALIZER_INTERNAL_H__
#define __SERIALIZER_INTERNAL_H__

#include <string_view>
#include <tuple>

#include "nlohmann/json.hpp"

#include "member_property.h"

namespace chaolib {
namespace serializer {

template <typename Class, typename T, typename... ALIAS>
constexpr auto make_member_property(T Class::* member, ALIAS... aliases)
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

using JSON = nlohmann::json;

template <typename T> void from_json(const JSON& json, T& object) {
  constexpr auto size =
      std::tuple_size<decltype(T::get_member_properties())>::value;
  chaolib::serializer::for_sequence(
      std::make_index_sequence<size>{},
      [&](auto i) {
        constexpr auto property = std::get<i>(T::get_member_properties());
        for (auto name : property.names_)
          if (json.contains(name)) {
            object.*(property.member_) = json[name];
            break;
          }
      });
}

template <typename T> T from_json(const JSON& json) {
  T result;
  from_json(json, result);
  return result;
}

template <typename T> void to_json(JSON& json, const T& object) {
  constexpr auto size =
      std::tuple_size<decltype(T::get_member_properties())>::value;
  chaolib::serializer::for_sequence(
      std::make_index_sequence<size>{},
      [&](auto i) {
        constexpr auto property = std::get<i>(T::get_member_properties());
        json[property.names_[0]] = object.*(property.member_);
      });
}

template <typename T> JSON to_json(const T& object) {
  JSON result;
  to_json(result, object);
  return result;
}

} // namespace serializer
} // namespace chaolib

#endif // __SERIALIZER_INTERNAL_H__
