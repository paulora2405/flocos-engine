#ifndef TEST_HPP
#define TEST_HPP

#include <functional>
#include <vector>

#include "logging/gl_error.hpp"

namespace tests {

class Test {
public:
  Test() {}
  virtual ~Test() {}

  virtual void onUpdate(float deltaTime) { deltaTime += 0; }
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
    LOG(TRACE) << "Registering test " << label;

    m_Tests.push_back(std::make_pair(label, []() { return new T(); }));
  }
};

}  // namespace tests

#endif