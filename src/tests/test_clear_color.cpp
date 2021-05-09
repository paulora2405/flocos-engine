#include "tests/test_clear_color.hpp"

#include "logging/gl_error.hpp"
#include "vendor/imgui/imgui.h"

namespace tests {

TestClearColor::TestClearColor() : m_ClearColor{0.5f, 0.2f, 0.8f, 1.0f} {
  LOG(DEBUG) << "Creating Clear Color Test";
}

TestClearColor::~TestClearColor() {}

void TestClearColor::onUpdate(float deltaTime) {
  deltaTime += 0;
}

void TestClearColor::onRender() {
  GLCALL(glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b,
                      m_ClearColor.a));
  GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void TestClearColor::onImGuiRender() {
  ImGui::ColorEdit4("Clear Color", &m_ClearColor[0]);
}

}  // namespace tests