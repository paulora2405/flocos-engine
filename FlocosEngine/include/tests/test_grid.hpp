#ifndef TEST_GRID_HPP
#define TEST_GRID_HPP

#include <memory>
#include <vector>

#include "graphics/index_buffer.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"
#include "graphics/vertex_buffer.hpp"
#include "tests/test.hpp"
#include "vendor/glm/glm.hpp"

namespace TEST {

class TestGrid : public Test {
private:
  u_short m_WinWidth;
  u_short m_WinHeight;
  const u_short m_GridM;
  const u_short m_GridN;
  glm::mat4 m_ProjMatrix;
  glm::mat4 m_ViewMatrix;
  std::vector<glm::vec3> m_Translations;
  std::vector<u_short> m_SelectedTexs;
  std::unique_ptr<GE::VertexArray> m_VAO;
  std::unique_ptr<GE::VertexBuffer> m_VB;
  std::unique_ptr<GE::IndexBuffer> m_IB;
  std::unique_ptr<GE::Shader> m_Shader;
  std::unique_ptr<GE::Texture> m_Texture;

public:
  TestGrid();
  ~TestGrid();

  void onUpdate(float &deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;
};

}  // namespace TEST

#endif