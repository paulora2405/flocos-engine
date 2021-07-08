#include "tests/test_batch_render.hpp"

#include "graphics/graphics_engine.hpp"
#include "graphics/renderer.hpp"
#include "graphics/vertex_buffer_layout.hpp"
#include "logging/gl_error.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"

namespace tests {

TestBatchRender::TestBatchRender()
    : m_WinWidth{GE::GraphicsEngine::getInstance().getWindowWidth()},
      m_WinHeight{GE::GraphicsEngine::getInstance().getWindowHeight()},
      m_GridM{2},
      m_GridN{2},
      m_ProjMatrix{glm::ortho(0.0f, (float)m_WinWidth, 0.0f, (float)m_WinHeight, -1.0f, 1.0f)},
      m_ViewMatrix{glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))} {
  LOG(DEBUG) << "Creating Batch Render Test";
  //

  float positions[] = {
      +0.0f,   +0.0f,   0.18f, 0.6f,  0.96f, 1.0f,  // i=0
      +100.0f, +0.0f,   0.18f, 0.6f,  0.96f, 1.0f,  // i=1
      +100.0f, +100.0f, 0.18f, 0.6f,  0.96f, 1.0f,  // i=2
      +0.0f,   +100.0f, 0.18f, 0.6f,  0.96f, 1.0f,  // i=3

      +200.0f, +0.0f,   1.0f,  0.93f, 0.24f, 1.0f,  // i=4
      +300.0f, +0.0f,   1.0f,  0.93f, 0.24f, 1.0f,  // i=5
      +300.0f, +100.0f, 1.0f,  0.93f, 0.24f, 1.0f,  // i=6
      +200.0f, +100.0f, 1.0f,  0.93f, 0.24f, 1.0f,  // i=7
  };
  unsigned int indices[] = {
      0, 1, 2, 2, 3, 0,  // 1st and 2nd triangles indices
      4, 5, 6, 6, 7, 4,
  };

  m_VAO = std::make_unique<GE::VertexArray>();

  /*                                          8 vertex with 6 floats each */
  m_VB = std::make_unique<GE::VertexBuffer>(positions, 8 * 6 * sizeof(float));

  GE::VertexBufferLayout layout;
  layout.push<float>(2);  // vec2 of float pos
  layout.push<float>(4);  // vec4 of rgba vals
  m_VAO->addBuffer(*m_VB, layout);

  /*                            12 indices will be drawn */
  m_IB = std::make_unique<GE::IndexBuffer>(indices, 12);

  m_Shader = std::make_unique<GE::Shader>("res/shaders/color_quad_vf.shader");
  m_Shader->bind();

  // m_Texture1 = std::make_unique<GE::Texture>("res/textures/c++.png");
  // m_Texture1->bind();
  // m_Shader->setUniform1i("u_Texture", 0);
}

TestBatchRender::~TestBatchRender() {}

void TestBatchRender::onUpdate(float deltaTime) {
  deltaTime += 0;
}

void TestBatchRender::onRender() {
  GLCALL(glClearColor(0.67f, 0.85f, 0.89f, 1.0f));
  GLCALL(glClear(GL_COLOR_BUFFER_BIT));

  GE::Renderer re;
  // m_Texture1->bind();

  glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), {200, 200, 0});
  /* P * V * M because opengl uses coloum-major matrices */
  glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model_matrix;

  m_Shader->bind();
  m_Shader->setUniformMat4f("u_MVP", mvp);
  re.draw(*m_VAO, *m_IB, *m_Shader);
}

void TestBatchRender::onImGuiRender() {
  // ImGui::SliderFloat("X1", (float*)&m_Translation1.x, 0.0f, (float)m_WinWidth);
  // ImGui::SliderFloat("Y1", (float*)&m_Translation1.y, 0.0f, (float)m_WinHeight);

  // ImGui::SliderFloat("X2", (float*)&m_Translation2.x, 0.0f, (float)m_WinWidth);
  // ImGui::SliderFloat("Y2", (float*)&m_Translation2.y, 0.0f, (float)m_WinHeight);
}

}  // namespace tests