#include <iostream>

#include "nlohmann/json.hpp"

#include "serializer/serializer.h"

struct Test {
  int a;
  double b;
  bool c;

  SERIALIZER(Test, a, b, c);
};

int main() {
  using namespace chaolib;
  Test t{1, 2, false};
  nlohmann::json json = chaolib::serializer::to_json(t);

  std::cout << json.dump() << std::endl;
  Test t2 = chaolib::serializer::from_json<Test>(json);
}