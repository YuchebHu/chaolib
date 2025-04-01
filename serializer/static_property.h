#ifndef __STATIC_PROPERTY_H__
#define __STATIC_PROPERTY_H__

#pragma once

#include <string_view>

namespace chaolib {
namespace serializer {
template <typename Class, typename T> struct StaticProperty {
  constexpr StaticProperty(T Class::*member, std::string_view name)
      : member_{member}, name_{name} {}

  using Type = T;

  T Class::*member_;
  std::string_view name_;
};

template <typename Class, typename T>
constexpr auto property(T Class::*member, std::string_view name) {
  return StaticProperty<Class, T>{member, name};
}

} // namespace serializer
} // namespace chaolib

#endif // __STATIC_PROPERTY_H__
