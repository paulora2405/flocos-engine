#include "tests/test_grid.hpp"

#include "graphics/graphics_engine.hpp"
#include "graphics/renderer.hpp"
#include "graphics/vertex_buffer_layout.hpp"
#include "logging/gl_error.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"

namespace TEST {

TestGrid::TestGrid()
    : m_WinWidth{GE::GraphicsEngine::getInstance().getWindowWidth()},
      m_WinHeight{GE::GraphicsEngine::getInstance().getWindowHeight()},
      m_GridM{30},
      m_GridN{30},
      m_ProjMatrix{glm::ortho(0.0f, (float)m_WinWidth, 0.0f, (float)m_WinHeight, -1.0f, 1.0f)},
      m_ViewMatrix{glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))},
      m_Translations{},
      m_SelectedTexs{} {
  LOG(DEBUG) << "Creating Grid Test";
  //

  float offset = (m_WinWidth <= m_WinHeight) * ((float)m_WinWidth / (float)m_GridM) +
                 (m_WinHeight < m_WinWidth) * ((float)m_WinHeight / (float)m_GridN);

  LOG(DEBUG) << "Grid " << m_GridM << 'x' << m_GridN << " (offset = " << offset << " pixels)";

  for(uint i = 0; i < m_GridM; i++)
    for(uint j = 0; j < m_GridN; j++)
      m_Translations.push_back(glm::vec3{offset * i, offset * j, 0});

  LOG(DEBUG) << "m_Translations.size() = " << m_Translations.size();

  float offX = (m_WinHeight < m_WinWidth) * ((m_WinWidth - m_WinHeight) / 2);
  float offY = (m_WinWidth <= m_WinHeight) * ((m_WinHeight - m_WinWidth) / 2);

  LOG(DEBUG) << "Initial Pos offset x-y: " << offX << '-' << offY;

  float positions[] = {
      offX,          offY,          0.0f, 0.0f,  // i=0 vec2 of pos |_ , vec2 of tex bounds
      offX + offset, offY,          1.0f, 0.0f,  // i=1 vec2 of pos _| , vec2 of tex bounds
      offX + offset, offY + offset, 1.0f, 1.0f,  // i=2 vec2 of pos ‾| , vec2 of tex bounds
      offX,          offY + offset, 0.0f, 1.0f,  // i=3 vec2 of pos |‾ , vec2 of tex bounds
  };
  unsigned int indices[] = {
      0, 1, 2, 2, 3, 0,  // 1st and 2nd triangles indices
  };

  m_VAO = std::make_unique<GE::VertexArray>();

  /*                                          4 vertex with 4 floats each */
  m_VB = std::make_unique<GE::VertexBuffer>(positions, 4 * 4 * sizeof(float));

  GE::VertexBufferLayout layout;
  layout.push<float>(2);  // vec2 of float pos
  layout.push<float>(2);  // vec2 of float tex bounds
  m_VAO->addBuffer(*m_VB, layout);

  /*                            6 indices will be drawn */
  m_IB = std::make_unique<GE::IndexBuffer>(indices, 6);

  m_Shader = std::make_unique<GE::Shader>("res/shaders/tex_quad_vf.shader");
  m_Shader->bind();

  m_Texture = std::make_unique<GE::Texture>("res/textures/test_square.png");
  m_Texture->bind();
  m_Shader->setUniform1i("u_Texture", 0);
}

TestGrid::~TestGrid() {}

void TestGrid::onUpdate(float &deltaTime) {
  deltaTime = deltaTime;
}

void TestGrid::onRender() {
  GLCALL(glClearColor(0.67f, 0.85f, 0.89f, 1.0f));
  GLCALL(glClear(GL_COLOR_BUFFER_BIT));

  GE::Renderer re;
  m_Texture->bind();

  for(uint i = 0; i < m_GridM * m_GridN; i++) {
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), m_Translations.at(i));
    /* P * V * M because opengl uses coloum-major matrices */
    glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model_matrix;

    m_Shader->bind();
    m_Shader->setUniformMat4f("u_MVP", mvp);
    re.draw(*m_VAO, *m_IB, *m_Shader);
  }
  //   }
  // }
}

void TestGrid::onImGuiRender() {
  // ImGui::SliderFloat("X1", (float*)&m_Translation1.x, 0.0f, (float)m_WinWidth);
  // ImGui::SliderFloat("Y1", (float*)&m_Translation1.y, 0.0f, (float)m_WinHeight);

  // ImGui::SliderFloat("X2", (float*)&m_Translation2.x, 0.0f, (float)m_WinWidth);
  // ImGui::SliderFloat("Y2", (float*)&m_Translation2.y, 0.0f, (float)m_WinHeight);
}

}  // namespace TEST