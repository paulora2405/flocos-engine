#ifndef TEST_TEXTURE_HPP
#define TEST_TEXTURE_HPP

#include <memory>

#include "graphics/index_buffer.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"
#include "graphics/vertex_buffer.hpp"
#include "tests/test.hpp"
#include "vendor/glm/glm.hpp"

namespace TEST {

class TestTexture2D : public Test {
private:
  unsigned short m_WinWidth;
  unsigned short m_WinHeight;
  glm::vec3 m_Translation1;
  glm::vec3 m_Translation2;
  std::unique_ptr<GE::VertexArray> m_VAO;
  std::unique_ptr<GE::VertexBuffer> m_VB;
  std::unique_ptr<GE::IndexBuffer> m_IB;
  std::unique_ptr<GE::Shader> m_Shader;
  std::unique_ptr<GE::Texture> m_Texture;
  glm::mat4 m_ProjMatrix;
  glm::mat4 m_ViewMatrix;

public:
  TestTexture2D();
  ~TestTexture2D();

  void onUpdate(float &deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;
};

}  // namespace TEST

#endif