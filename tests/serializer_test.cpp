#include <array>
#include <iostream>
#include <string_view>
#include <tuple>

#include "serializer/serializer.h"

struct Test {
  int a_;
  double b_;
  bool c_;

  SERIALIZER_WITH_ALIAS(
      Test,
      MAKE_ALIAS(a_, "A", "a_"),
      MAKE_ALIAS(b_, "b_", "B"),
      MAKE_ALIAS(c_, "c_", "C"));
};

struct TestB {
  int a_;
  double b_;
  bool c_;

  SERIALIZER(TestB, a_, b_, c_);
};

int main() {
  using namespace chaolib::serializer;
  {
    nlohmann::json newTest;
    newTest["A"] = 10;
    newTest["B"] = 2.0;
    newTest["C"] = false;
    Test a = chaolib::serializer::from_json<Test>(newTest);
    nlohmann::json json = to_json(a);
    std::cout << json.dump() << std::endl;
  }
  {
    Test t{1, 2, false};
    nlohmann::json json = to_json(t);
    std::cout << json.dump() << std::endl;
    Test t2 = from_json<Test>(json);
    std::cout << to_json<Test>(t2).dump() << std::endl;
  }
}