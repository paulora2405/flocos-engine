#ifndef TEST_HPP
#define TEST_HPP

#include <functional>
#include <vector>

#include "logging/gl_error.hpp"

namespace TEST {

class Test {
public:
  Test() {}
  virtual ~Test() {}

  virtual void onUpdate(float &deltaTime) { deltaTime = deltaTime; }
  virtual void onRender() {}
  virtual void onImGuiRender() {}
};

class TestMenu : public Test {
private:
  Test *&m_CurrentTest;
  std::vector<std::pair<std::string, std::function<Test *()>>> m_Tests;

public:
  TestMenu(Test *&currentTestPointer);

  void onImGuiRender() override;

  template <typename T>
  void registerTest(const std::string &label) {
    LOG(DEBUG) << "Registering test " << label;

    m_Tests.push_back(std::make_pair(label, []() { return new T(); }));
  }
};

}  // namespace TEST

#endif