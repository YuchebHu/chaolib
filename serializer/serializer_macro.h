#ifndef __SERIALIZER_MACRO_H__
#define __SERIALIZER_MACRO_H__

#include "static_property.h"

#define EXPAND(...) __VA_ARGS__
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME

#define MAKE_PROPERTY(...)
#define MAKE_PROPERTY_1(a) chaolib::serializer::property(&THIS::a, #a)
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

#endif // __SERIALIZER_MACRO_H__
