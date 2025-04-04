#ifndef __NLOHMANN_ADL_H__
#define __NLOHMANN_ADL_H__
#include <type_traits>

#include "nlohmann/json.hpp"

#include "has_properties.h"
#include "serializer_internal.h"

namespace nlohmann {
template <
    typename T,
    typename std::enable_if_t<chaolib::HasProperties<T>::value, int> = 0>
static void to_json(json& json_object, const T& object) {
  constexpr auto size =
      std::tuple_size<decltype(T::get_member_properties())>::value;
  chaolib::serializer::for_sequence(
      std::make_index_sequence<size>{},
      [&](auto i) {
        constexpr auto property = std::get<i>(T::get_member_properties());
        json_object[property.names_[0]] = object.*(property.member_);
      });
}

template <
    typename T,
    typename std::enable_if_t<chaolib::HasProperties<T>::value, int> = 0>
static void from_json(const json& json_object, T& object) {
  constexpr auto size =
      std::tuple_size<decltype(T::get_member_properties())>::value;
  chaolib::serializer::for_sequence(
      std::make_index_sequence<size>{},
      [&](auto i) {
        constexpr auto property = std::get<i>(T::get_member_properties());
        for (auto name : property.names_) {
          if (json_object.contains(name)) {
            using Type =
                std::remove_reference_t<decltype(object.*(property.member_))>;
            object.*(property.member_) = json_object[name].get<Type>();
            break;
          }
        }
      });
}
} // namespace nlohmann

#endif // __NLOHMANN_ADL_H__