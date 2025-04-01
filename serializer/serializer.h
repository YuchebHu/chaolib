#ifndef __SERIALIZER_H__
#define __SERIALIZER_H__

#include <tuple>

#include "nlohmann/json.hpp"

#include "serializer_macro.h"
#include "static_property.h"

namespace chaolib {
namespace serializer {
template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
  (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
}

using JSON = nlohmann::json;

template <typename T> void from_json(const JSON& json, T& object) {
  constexpr auto size = std::tuple_size<decltype(T::kProperties)>::value;
  chaolib::serializer::for_sequence(
      std::make_index_sequence<size>{},
      [&](auto i) {
        constexpr auto property = std::get<i>(T::kProperties);
        if (json.contains(property.name_)) {
          object.*(property.member_) = json[property.name_];
        }
      });
}

template <typename T> T from_json(const JSON& json) {
  T result;
  from_json(json, result);
  return result;
}

template <typename T> void to_json(JSON& json, const T& object) {
  constexpr auto size = std::tuple_size<decltype(T::kProperties)>::value;
  chaolib::serializer::for_sequence(
      std::make_index_sequence<size>{},
      [&](auto i) {
        constexpr auto property = std::get<i>(T::kProperties);
        json[property.name_] = object.*(property.member_);
      });
}

template <typename T> JSON to_json(const T& object) {
  JSON result;
  to_json(result, object);
  return result;
}

} // namespace serializer
} // namespace chaolib

#endif // __SERIALIZER_H__
