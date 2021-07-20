#ifndef TEST_ANTS_HPP
#define TEST_ANTS_HPP

#include <memory>

#include "graphics/index_buffer.hpp"
#include "graphics/shader.hpp"
#include "graphics/vertex_array.hpp"
#include "graphics/vertex_buffer.hpp"
#include "graphics/vertex_buffer_layout.hpp"
#include "simulation/colony.hpp"
#include "tests/test.hpp"
#include "vendor/glm/glm.hpp"

namespace TEST {

class TestAnts : public Test {
private:
  u_short m_WinWidth;
  u_short m_WinHeight;
  u_short m_GridM;
  u_short m_GridN;
  uint m_AliveQnt;
  uint m_DeadQnt;
  uint m_VisionRadius;
  uint m_Iterations;
  std::unique_ptr<float[]> m_Positions;
  uint m_PosSize;
  std::unique_ptr<uint[]> m_Indices;
  uint m_IndSize;
  bool m_Initiated;
  bool m_Paused;
  bool m_HideAlive;
  bool m_Finishing;
  bool m_Finished;
  glm::mat4 m_ProjMatrix;
  glm::mat4 m_ViewMatrix;
  std::unique_ptr<SIM::Colony> m_Colony;
  std::unique_ptr<GE::VertexArray> m_VAO;
  std::unique_ptr<GE::VertexBuffer> m_VB;
  std::unique_ptr<GE::VertexBufferLayout> m_VBL;
  std::unique_ptr<GE::IndexBuffer> m_IB;
  std::unique_ptr<GE::Shader> m_Shader;

public:
  TestAnts();
  ~TestAnts();

  void init();
  void updateGrid();
  void saveGridToFile();

  void onUpdate(float &deltaTime) override;
  void onRender() override;
  void onImGuiRender() override;
};

}  // namespace TEST

#endif