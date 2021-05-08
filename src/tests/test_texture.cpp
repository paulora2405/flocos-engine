#include "tests/test_texture.hpp"

#include "graphics/graphics_engine.hpp"
#include "graphics/renderer.hpp"
#include "graphics/vertex_buffer_layout.hpp"
#include "logging/gl_error.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"

namespace tests {

TestTexture2D::TestTexture2D()
    : m_WinWidth{GE::GraphicsEngine::getInstance().getWindowWidth()},
      m_WinHeight{GE::GraphicsEngine::getInstance().getWindowHeight()},
      m_Translation1{200, 200, 0},
      m_Translation2{400, 200, 0},
      m_ProjMatrix{glm::ortho(
          0.0f, (float)m_WinWidth, 0.0f, (float)m_WinHeight, -1.0f, 1.0f)},
      m_ViewMatrix{glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))} {
  LOG(DEBUG) << "Creating Texture 2D Test";
  //
  float positions[] = {
      -50.0f, -50.0f, 0.0f, 0.0f,  // i=0 vec2 of pos, vec2 of tex bounds
      +50.0f, -50.0f, 1.0f, 0.0f,  // i=1 vec2 of pos, vec2 of tex bounds
      +50.0f, +50.0f, 1.0f, 1.0f,  // i=2 vec2 of pos, vec2 of tex bounds
      -50.0f, +50.0f, 0.0f, 1.0f   // i=3 vec2 of pos, vec2 of tex bounds
  };
  unsigned int indices[] = {
      0, 1, 2,  // 1st triangle indices of positions array
      2, 3, 0,  // 2nd triangle indices of positions array
  };

  m_VAO = std::make_unique<GE::VertexArray>();

  /*                                          4 vertex with 4 floats each */
  m_VB = std::make_unique<GE::VertexBuffer>(positions, 4 * 4 * sizeof(float));

  GE::VertexBufferLayout layout;
  layout.push<float>(2);  // vec2 of float
  layout.push<float>(2);  // vec2 of float
  m_VAO->addBuffer(*m_VB, layout);

  /*                            6 indices will be drawn */
  m_IB = std::make_unique<GE::IndexBuffer>(indices, 6);

  m_Shader = std::make_unique<GE::Shader>("res/shaders/triangle_vf.shader");
  m_Shader->bind();

  m_Texture = std::make_unique<GE::Texture>("res/textures/grass.png");
  m_Texture->bind();
  m_Shader->setUniform1i("u_Texture", 0);
}

TestTexture2D::~TestTexture2D() {}

void TestTexture2D::onUpdate(float deltaTime) {
  deltaTime += 0;
}

void TestTexture2D::onRender() {
  GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  GLCALL(glClear(GL_COLOR_BUFFER_BIT));

  GE::Renderer re;
  m_Texture->bind();

  {
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), m_Translation1);
    /* P * V * M because opengl uses coloum-major matrices */
    glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model_matrix;

    m_Shader->bind();
    m_Shader->setUniformMat4f("u_MVP", mvp);
    re.draw(*m_VAO, *m_IB, *m_Shader);
  }
  {
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), m_Translation2);
    /* P * V * M because opengl uses coloum-major matrices */
    glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model_matrix;
    m_Shader->bind();
    m_Shader->setUniformMat4f("u_MVP", mvp);
    re.draw(*m_VAO, *m_IB, *m_Shader);
  }
}

void TestTexture2D::onImGuiRender() {
  ImGui::SliderFloat("X1", (float*)&m_Translation1.x, 0.0f, (float)m_WinWidth);
  ImGui::SliderFloat("Y1", (float*)&m_Translation1.y, 0.0f, (float)m_WinHeight);

  ImGui::SliderFloat("X2", (float*)&m_Translation2.x, 0.0f, (float)m_WinWidth);
  ImGui::SliderFloat("Y2", (float*)&m_Translation2.y, 0.0f, (float)m_WinHeight);
}

}  // namespace tests