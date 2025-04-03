#include <iostream>

#include "serializer/serializer.h"

struct TestB {
  int a_;
  double b_;
  bool c_;

  SERIALIZER(TestB, a_, b_, c_);
};

int main() {
  using namespace chaolib::serializer;

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

  struct ArrayTest {
    std::vector<Test> test_array_;
    std::string name_;
    SERIALIZER(ArrayTest, test_array_, name_);
  };
  {
    constexpr auto properties = ArrayTest::get_member_properties();
    ArrayTest test{{{1, 2.0, false}}, "array test"};
    nlohmann::json jTest = test;
    std::cout << jTest.dump() << std::endl;
    ArrayTest testFromJson = jTest;
  }
  {
    nlohmann::json newTest;
    newTest["A"] = 10;
    newTest["B"] = 2.0;
    newTest["C"] = false;
    Test a = newTest;
    nlohmann::json json = a;
    std::cout << json.dump() << std::endl;
  }
  {
    Test t{1, 2, false};
    nlohmann::json json = t;
    std::cout << json.dump() << std::endl;
    Test t2 = json;
    json = t2;
    std::cout << json.dump() << std::endl;
  }
  {
    TestB t{1, 2, false};
    nlohmann::json json = t;
    std::cout << json.dump() << std::endl;
    TestB t2 = json;
    t2.a_ = 10;
    t2.c_ = true;
    json = t2;
    std::cout << json.dump() << std::endl;
  }
}