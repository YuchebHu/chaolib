#ifndef __SERIALIZER_MACRO_H__
#define __SERIALIZER_MACRO_H__

#include "serializer_internal.h"
#include "static_property.h"

#define EXPAND(...) __VA_ARGS__
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME

#define MAKE_MEMBER_PROPERTY(...)
#define MAKE_MEMBER_PROPERTY_1(a)                                              \
  chaolib::serializer::make_member_property(&THIS::a, #a)
#define MAKE_MEMBER_PROPERTY_2(a, ...)                                         \
  EXPAND(MAKE_MEMBER_PROPERTY_1(a), MAKE_MEMBER_PROPERTY_1(__VA_ARGS__))
#define MAKE_MEMBER_PROPERTY_3(a, ...)                                         \
  EXPAND(MAKE_MEMBER_PROPERTY_1(a), MAKE_MEMBER_PROPERTY_2(__VA_ARGS__))
#define MAKE_MEMBER_PROPERTY_4(a, ...)                                         \
  MAKE_MEMBER_PROPERTY_1(a), MAKE_MEMBER_PROPERTY_3(EXPAND(__VA_ARGS__))
#define MAKE_MEMBER_PROPERTY_5(a, ...)                                         \
  MAKE_MEMBER_PROPERTY_1(a), MAKE_MEMBER_PROPERTY_4(EXPAND(__VA_ARGS__))
#define MAKE_MEMBER_PROPERTY_6(a, ...)                                         \
  MAKE_MEMBER_PROPERTY_1(a), MAKE_MEMBER_PROPERTY_5(EXPAND(__VA_ARGS__))

#define SERIALIZER(TYPE, ...)                                                  \
  using THIS = TYPE;                                                           \
  constexpr static auto kProperties = std::make_tuple(EXPAND(GET_MACRO(        \
      __VA_ARGS__,                                                             \
      MAKE_MEMBER_PROPERTY_6,                                                  \
      MAKE_MEMBER_PROPERTY_5,                                                  \
      MAKE_MEMBER_PROPERTY_4,                                                  \
      MAKE_MEMBER_PROPERTY_3,                                                  \
      MAKE_MEMBER_PROPERTY_2,                                                  \
      MAKE_MEMBER_PROPERTY_1)(__VA_ARGS__)));

#define MAKE_ALIAS(a, ...)                                                     \
  chaolib::serializer::make_member_property(&THIS::a, #a, __VA_ARGS__)

#define SERIALIZER_WITH_ALIAS(TYPE, ...)                                       \
  using THIS = TYPE;                                                           \
  constexpr static auto kProperties = std::make_tuple(__VA_ARGS__);

#endif // __SERIALIZER_MACRO_H__
