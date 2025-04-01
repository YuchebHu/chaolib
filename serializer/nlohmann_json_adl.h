#pragma once
#include "nlohmann/json.hpp"

namespace chaolib {
namespace adl_wrapper {
template <typename T> struct NlohmannWrapper {
  NlohmannWrapper(T&& value) : value_(value) {}
  void from_json(const nlohmann::json& json) {
    T::from_json(json, this->value_);
  }
  void to_json(nlohmann::json& json) { T::to_json(json, this->value_); }

  T& value_;
};
} // namespace adl_wrapper
template <typename T> using NlohmannWrapper = adl_wrapper::NlohmannWrapper<T>;

} // namespace chaolib

namespace nlohmann {
template <typename T> struct adl_serializer<chaolib::NlohmannWrapper<T>> {
  static void from_json(const json& json, chaolib::NlohmannWrapper<T>& object) {
    object.from_json(json);
  }

  static void to_json(json& json, const chaolib::NlohmannWrapper<T>& object) {
    object.to_json(json);
  }
};

} // namespace nlohmann