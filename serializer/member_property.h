#ifndef __STATIC_PROPERTY_H__
#define __STATIC_PROPERTY_H__

#pragma once

#include <array>
#include <string_view>

namespace chaolib {
namespace serializer {
template <typename Class, typename T, int N> struct MemberProperty {
  constexpr MemberProperty(
      T Class::* member,
      std::array<std::string_view, N> names)
      : member_(member), names_(std::move(names)) {}

  T Class::* member_;
  std::array<std::string_view, N> names_;
};

} // namespace serializer
} // namespace chaolib

#endif // __STATIC_PROPERTY_H__
