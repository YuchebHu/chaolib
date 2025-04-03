#ifndef __HAS_PROPERTIES_H__
#define __HAS_PROPERTIES_H__

#include <type_traits>

namespace chaolib {
namespace serializer {

template <typename, typename = void> struct HasProperties : std::false_type {};

template <typename T>
struct HasProperties<
    T,
    std::void_t<decltype(std::declval<T>().get_member_properties())>>
    : std::true_type {};

} // namespace serializer
template <typename T> using HasProperties = serializer::HasProperties<T>;

} // namespace chaolib

#endif // __HAS_PROPERTIES_H__