#pragma once
#include "macro.h"
#include "static_property.h"

#define MAKE_PROPERTY(...)
#define MAKE_PROPERTY_1(a) chelib::serializer::property(&THIS::a, #a)
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

#define SERIALIZE_JSON(Type)                                                   \
  template <typename JSON, typename T = Type>                                  \
  static void from_json(const JSON& json, Type& object) {                      \
    constexpr auto size = std::tuple_size<decltype(Type::kProperties)>::value; \
    chelib::serializer::for_sequence(                                          \
        std::make_index_sequence<size>{},                                      \
        [&](auto i) {                                                          \
          constexpr auto property = std::get<i>(Type::kProperties);            \
          if (json.contains(property.name_)) {                                 \
            object.*(property.member_) = json[property.name_];                 \
          }                                                                    \
        });                                                                    \
  }                                                                            \
                                                                               \
  template <typename JSON, typename T = Type>                                  \
  static void to_json(JSON& json, const Type& object) {                        \
    constexpr auto size = std::tuple_size<decltype(Type::kProperties)>::value; \
    chelib::serializer::for_sequence(                                          \
        std::make_index_sequence<size>{},                                      \
        [&](auto i) {                                                          \
          constexpr auto property = std::get<i>(Type::kProperties);            \
          json[property.name_] = object.*(property.member_);                   \
        });                                                                    \
  }

#define SERIALIZER(TYPE, ...)                                                  \
  using THIS = TYPE;                                                           \
  constexpr static auto kProperties = std::make_tuple(EXPAND(GET_MACRO(        \
      __VA_ARGS__,                                                             \
      MAKE_PROPERTY_6,                                                         \
      MAKE_PROPERTY_5,                                                         \
      MAKE_PROPERTY_4,                                                         \
      MAKE_PROPERTY_3,                                                         \
      MAKE_PROPERTY_2,                                                         \
      MAKE_PROPERTY_1)(__VA_ARGS__)));                                         \
  SERIALIZE_JSON(THIS)