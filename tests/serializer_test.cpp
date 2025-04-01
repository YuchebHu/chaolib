#include <iostream>

#include "serializer/serializer.h"

struct Test {
  int a;
  double b;
  bool c;

  SERIALIZER(Test, a, b, c);
};

int main() {
  using namespace chelib;
  Test t{1, 2, false};
  nlohmann::json json = chelib::serializer::to_json(t);

  std::cout << json.dump() << std::endl;
  Test t2 = chelib::serializer::from_json<Test>(json);
}