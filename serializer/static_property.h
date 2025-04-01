#pragma once

#include <string_view>

namespace chelib {
namespace serializer {
template <typename Class, typename T> struct StaticProperty {
  constexpr StaticProperty(T Class::* member, std::string_view name)
      : member_{member}, name_{name} {}

  using Type = T;

  T Class::* member_;
  std::string_view name_;
};

template <typename Class, typename T>
constexpr auto property(T Class::* member, std::string_view name) {
  return StaticProperty<Class, T>{member, name};
}

} // namespace serializer
} // namespace chelib