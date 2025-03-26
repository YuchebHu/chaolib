#include <string_view>
#include <tuple>

#include "nlohmann/json.hpp"

template <typename Class, typename T> struct PropertyImpl {
  constexpr PropertyImpl(T Class::* member, std::string_view name)
      : member_{member}, name_{name} {}

  using Type = T;

  T Class::* member_;
  std::string_view name_;
};

template <typename Class, typename T>
constexpr auto property(T Class::* member, std::string_view name) {
  return PropertyImpl<Class, T>{member, name};
}

template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
  (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
}

#define EXPAND(...) __VA_ARGS__

#define MAKE_PROPERTY(...)
#define MAKE_PROPERTY_1(a) property(&THIS::a, #a)
#define MAKE_PROPERTY_2(a, ...)                                                \
  EXPAND(MAKE_PROPERTY_1(a), MAKE_PROPERTY_1(__VA_ARGS__))
#define MAKE_PROPERTY_3(a, ...)                                                \
  EXPAND(MAKE_PROPERTY_1(a), MAKE_PROPERTY_2(__VA_ARGS__))
#define MAKE_PROPERTY_4(a, ...)                                                \
  MAKE_PROPERTY_1(a), MAKE_PROPERTY_3(EXPAND(__VA_ARGS__))
#define MAKE_PROPERTY_5(a, ...)                                                \
  MAKE_PROPERTY_1(a), MAKE_PROPERTY_4(EXPAND(__VA_ARGS__))
#define MAKE_PROPERTY_6(a, ...)                                                \
  MAKE_PROPERTY_1(a), MAKE_PROPERTY_5(EXPAND(__VA_ARGS__))

#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME
#define SERIALIZER(TYPE, ...)                                                  \
  using THIS = TYPE;                                                           \
  constexpr static auto kProperties = std::make_tuple(EXPAND(GET_MACRO(        \
      __VA_ARGS__,                                                             \
      MAKE_PROPERTY_6,                                                         \
      MAKE_PROPERTY_5,                                                         \
      MAKE_PROPERTY_4,                                                         \
      MAKE_PROPERTY_3,                                                         \
      MAKE_PROPERTY_2,                                                         \
      MAKE_PROPERTY_1)(__VA_ARGS__)));

template <typename T> nlohmann::json to_json(const T& a) {
  constexpr auto nbProperties =
      std::tuple_size<decltype(T::kProperties)>::value;
  nlohmann::json json;
  // We iterate on the index sequence of size `nbProperties`
  for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
    // get the property
    constexpr auto property = std::get<i>(T::kProperties);

    // set the value to the member
    json[property.name_] = a.*(property.member_);
  });
  return json;
}

template <typename T> T from_json(const nlohmann::json& json) {
  T object;
  constexpr auto nbProperties =
      std::tuple_size<decltype(T::kProperties)>::value;
  // We iterate on the index sequence of size `nbProperties`
  for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
    // get the property
    constexpr auto property = std::get<i>(T::kProperties);

    // set the value to the member
    object.*(property.member_) = json[property.name_];
  });
  return object;
}

struct Test {
  int a;
  double b;
  bool c;

  SERIALIZER(Test, a, b, c)
};
