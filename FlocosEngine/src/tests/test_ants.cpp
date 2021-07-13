#include "tests/test_ants.hpp"

#include <random>

#include "graphics/graphics_engine.hpp"
#include "graphics/renderer.hpp"
#include "logging/gl_error.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"

namespace tests {

void TestAnts::init() {
  LOG(INFO) << "Initializing Ants Simulation Test";
  LOG(DEBUG) << "Initializing Ants Simulation Test";
  float offX = (m_WinHeight < m_WinWidth) * ((m_WinWidth - m_WinHeight) / 2);
  float offY = (m_WinWidth <= m_WinHeight) * ((m_WinHeight - m_WinWidth) / 2);
  float cellOffset = (m_WinWidth <= m_WinHeight) * ((float)m_WinWidth / (float)m_GridM) +
                     (m_WinHeight < m_WinWidth) * ((float)m_WinHeight / (float)m_GridN);

  size_t cellsTotal = m_GridM * m_GridN;
  m_PosSize = cellsTotal * 4 * 6;
  m_IndSize = cellsTotal * 6;

  m_Colony = std::make_unique<SIM::Colony>(m_GridM, m_GridN, m_AliveQnt, m_DeadQnt, m_VisionRadius);

  m_Positions = std::make_unique<float[]>(m_PosSize);
  m_Indices = std::make_unique<uint[]>(m_IndSize);

  uint n = 0, m = 0;
  for(size_t i = 0; i < m_PosSize; i += 4 * 6) {
    // vec4 of rgba
    SIM::GridState state = m_Colony.get()->query(m, n);
    float r, g, b, a;

    if(state == SIM::GridState::Empty)
      r = g = b = a = 1.0f;
    if(state == SIM::GridState::AliveFree)
      r = 0.0f, g = 1.0f, b = 0.0f, a = 1.0f;
    if(state == SIM::GridState::DeadFree)
      r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;
    if(state == SIM::GridState::BothFree)
      r = 1.0f, g = 1.0f, b = 0.0f, a = 1.0f;
    if(state == SIM::GridState::AliveBusy)
      r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;

    for(size_t j = 0; j < 4; j++) {
      m_Positions[i + 2 + j * 6] = r;
      m_Positions[i + 3 + j * 6] = g;
      m_Positions[i + 4 + j * 6] = b;
      m_Positions[i + 5 + j * 6] = a;
    }

    // vec2 of positions
    m_Positions[i + 0] = offX;  //               index=0 |_
    m_Positions[i + 1] = offY;
    m_Positions[i + 6] = offX + cellOffset;  //  index=1 _|
    m_Positions[i + 7] = offY;
    m_Positions[i + 12] = offX + cellOffset;  // index=2 ‾|
    m_Positions[i + 13] = offY + cellOffset;
    m_Positions[i + 18] = offX;  //              index=3 |‾
    m_Positions[i + 19] = offY + cellOffset;

    if(++n >= m_GridN) {
      offX += cellOffset;
      offY = (m_WinWidth <= m_WinHeight) * ((m_WinHeight - m_WinWidth) / 2);
      n = 0;
      m++;
    } else {
      offY += cellOffset;
    }
  }

  uint indStep = 0;
  for(size_t i = 0; i < m_IndSize; i += 6) {
    m_Indices[i + 0] = 0 + indStep;
    m_Indices[i + 1] = 1 + indStep;
    m_Indices[i + 2] = 2 + indStep;
    m_Indices[i + 3] = 2 + indStep;
    m_Indices[i + 4] = 3 + indStep;
    m_Indices[i + 5] = 0 + indStep;
    indStep += 4;
  }

  m_VAO = std::make_unique<GE::VertexArray>();
  m_VB = std::make_unique<GE::VertexBuffer>(m_Positions.get(), m_PosSize * sizeof(float));
  m_VBL = std::make_unique<GE::VertexBufferLayout>();
  m_VBL->push<float>(2);  // vec2 of float pos
  m_VBL->push<float>(4);  // vec4 of rgba vals
  m_VAO->addBuffer(*m_VB, *m_VBL);
  m_IB = std::make_unique<GE::IndexBuffer>(m_Indices.get(), m_IndSize);
  m_Shader = std::make_unique<GE::Shader>("res/shaders/color_quad_vf.shader");
  m_Shader->bind();
}

//

void TestAnts::updateGrid() {
  // m_Colony = std::make_unique<SIM::Colony>(m_GridM, m_GridN, m_AliveQnt, m_DeadQnt,
  // m_VisionRadius);

  m_Colony.get()->iterate();

  uint n = 0, m = 0;
  for(size_t i = 0; i < m_PosSize; i += 4 * 6) {
    // vec4 of rgba
    SIM::GridState state = m_Colony.get()->query(m, n);
    // LOG(DEBUG) << "(M,N) = " << m << ',' << n << " -> " << (uint)state;
    float r, g, b, a;

    if(state == SIM::GridState::Empty)
      r = g = b = a = 1.0f;
    if(state == SIM::GridState::AliveFree)
      r = 0.0f, g = 1.0f, b = 0.0f, a = 1.0f;
    if(state == SIM::GridState::DeadFree)
      r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;
    if(state == SIM::GridState::BothFree)
      r = 1.0f, g = 1.0f, b = 0.0f, a = 1.0f;
    if(state == SIM::GridState::AliveBusy)
      r = 1.0f, g = 0.0f, b = 0.0f, a = 1.0f;

    for(size_t j = 0; j < 4; j++) {
      m_Positions[i + 2 + j * 6] = r;
      m_Positions[i + 3 + j * 6] = g;
      m_Positions[i + 4 + j * 6] = b;
      m_Positions[i + 5 + j * 6] = a;
    }
    if(++n >= m_GridN)
      n = 0, m++;
  }
  m_VB = std::make_unique<GE::VertexBuffer>(m_Positions.get(), m_PosSize * sizeof(float));
  m_VAO->addBuffer(*m_VB, *m_VBL);
}

//

void TestAnts::onUpdate(float &deltaTime) {
  deltaTime = deltaTime;
  static uint tick = 1;
  if(!m_Paused and m_Initiated) {
    if(tick++ >= 5) {
      updateGrid();
      tick = 1;
    }
  }
}

//

void TestAnts::onRender() {
  // GLCALL(glClearColor(0.67f, 0.85f, 0.89f, 1.0f));
  // GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  // GLCALL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
  GLCALL(glClearColor(0.85f, 0.96f, 0.65f, 1.0f));
  GLCALL(glClear(GL_COLOR_BUFFER_BIT));
  if(m_Initiated) {
    GE::Renderer re;
    // m_Texture1->bind();

    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), {0, 0, 0});
    /* P * V * M because opengl uses coloum-major matrices */
    glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model_matrix;

    m_Shader->bind();
    m_Shader->setUniformMat4f("u_MVP", mvp);
    re.draw(*m_VAO, *m_IB, *m_Shader);
  }
}

//

void TestAnts::onImGuiRender() {
  if(!m_Initiated) {
    ImGui::Text("Enter the size MxN of the Grid:");
    if(ImGui::IsItemHovered())
      ImGui::SetTooltip("If either input is left empty, the default grid will be initialized.");
    static char mSize[5] = "";
    static char nSize[5] = "";
    static char aliveAntsQnt[5] = "";
    static char deadAntsQnt[5] = "";
    static char antsVisionRadius[5] = "";
    ImGui::PushItemWidth(40);
    ImGui::InputTextWithHint("x", std::to_string(m_GridM).c_str(), mSize, 5,
                             ImGuiInputTextFlags_CharsDecimal);
    ImGui::SameLine();
    ImGui::InputTextWithHint(" ", std::to_string(m_GridN).c_str(), nSize, 5,
                             ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopItemWidth();
    ImGui::PushItemWidth(90);
    ImGui::InputTextWithHint("Alive", std::to_string(m_AliveQnt).c_str(), aliveAntsQnt, 5,
                             ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputTextWithHint("Dead", std::to_string(m_DeadQnt).c_str(), deadAntsQnt, 5,
                             ImGuiInputTextFlags_CharsDecimal);
    ImGui::InputTextWithHint("Vision", std::to_string(m_VisionRadius).c_str(), antsVisionRadius, 5,
                             ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopItemWidth();

    if(ImGui::Button("Confirm")) {
      char empty[] = "";
      if(strcmp(mSize, empty) and strcmp(nSize, empty)) {
        m_GridM = atoi(mSize);
        m_GridN = atoi(nSize);
        m_AliveQnt = strcmp(aliveAntsQnt, empty) ? atoi(aliveAntsQnt) : (m_GridM * m_GridN) / 8;
        m_DeadQnt = strcmp(deadAntsQnt, empty) ? atoi(deadAntsQnt) : (m_GridM * m_GridN) / 8;
        m_VisionRadius = strcmp(antsVisionRadius, empty) ? atoi(antsVisionRadius) : 2;
      }
      m_Initiated = true;
      // LOG(DEBUG) << "Colony Param: " << m_GridM << ',' << m_GridN << ',' << m_AliveQnt << ','
      //            << m_DeadQnt << ',' << m_VisionRadius;
      this->init();
    }
    // ImGui::TextDisabled("If either input is left empty,\n the default grid will be
    // initialized.");
  } else {
    static std::string pauseText{"Start"};
    if(ImGui::Button(pauseText.c_str())) {
      m_Paused = !m_Paused;
      if(m_Paused)
        pauseText = "Continue";
      else
        pauseText = "Pause";
    }
    if(m_Paused)
      if(ImGui::Button("Update One Tick"))
        this->updateGrid();
  }
}

//

TestAnts::TestAnts()
    : m_WinWidth{GE::GraphicsEngine::getInstance().getWindowWidth()},
      m_WinHeight{GE::GraphicsEngine::getInstance().getWindowHeight()},
      m_GridM{200},
      m_GridN{200},
      m_AliveQnt{5000},
      m_DeadQnt{5000},
      m_VisionRadius{2},
      m_Positions{},
      m_PosSize{0},
      m_Indices{},
      m_IndSize{0},
      m_Initiated{false},
      m_Paused{true},
      m_ProjMatrix{glm::ortho(0.0f, (float)m_WinWidth, 0.0f, (float)m_WinHeight, -1.0f, 1.0f)},
      m_ViewMatrix{glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))},
      m_Colony{} {
  LOG(DEBUG) << "Creating Ants Test";
}

//

TestAnts::~TestAnts() {}

}  // namespace tests