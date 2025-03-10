#include <memory>

#include "core/noncopyable.h"

class CannotCopy : private chelib::Noncopyable {
public:
  explicit CannotCopy(int a) : a_(a) {}
  CannotCopy(CannotCopy &&ccp) : a_(std::move(ccp.a_)) {}
  CannotCopy &operator=(CannotCopy &&ccp) {
    this->a_ = std::move(ccp.a_);
    return *this;
  }

private:
  int a_;
};

int main() {
  CannotCopy ccp{1};
  CannotCopy ccp2{1};
  auto moveccp = std::move(ccp);
  ccp2 = std::move(ccp);

  // CannotCopy ccp3{ccp2};
  // auto copyccp = ccp3;
  return 0;
}