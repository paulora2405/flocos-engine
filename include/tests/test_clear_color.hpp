#ifndef TEST_CLEAR_COLOR_HPP
#define TEST_CLEAR_COLOR_HPP

#include "tests/test.hpp"
#include "vendor/glm/glm.hpp"

namespace tests {

class TestClearColor : public Test {
private:
  glm::vec4 m_ClearColor;

public:
  TestClearColor();
  ~TestClearColor();

  void onUpdate(float deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;
};

}  // namespace tests

#endif