#include "tests/test_batch_render.hpp"

#include <array>
#include <random>

#include "graphics/graphics_engine.hpp"
#include "graphics/renderer.hpp"
#include "graphics/vertex_buffer_layout.hpp"
#include "logging/gl_error.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"

namespace TEST {

void TestBatchRender::init() {
  LOG(DEBUG) << "Initiated BatchRender";
  float offX = (m_WinHeight < m_WinWidth) * ((m_WinWidth - m_WinHeight) / 2);
  float offY = (m_WinWidth <= m_WinHeight) * ((m_WinHeight - m_WinWidth) / 2);
  float cellOffset = (m_WinWidth <= m_WinHeight) * ((float)m_WinWidth / (float)m_GridM) +
                     (m_WinHeight < m_WinWidth) * ((float)m_WinHeight / (float)m_GridN);

  uint cellsTotal = m_GridM * m_GridN;
  m_PosSize = cellsTotal * 4 * 6;
  m_IndSize = cellsTotal * 6;

  LOG(DEBUG) << "CellsTotal = " << cellsTotal << " ; PosSize = " << m_PosSize
             << " ; IndSize = " << m_IndSize;
  LOG(DEBUG) << "Initial Pos cellOffset x-y: " << offX << '-' << offY;
  LOG(DEBUG) << "Grid " << m_GridM << 'x' << m_GridN << " (cellOffset = " << cellOffset
             << " pixels)";

  m_Positions = std::make_unique<float[]>(m_PosSize);
  m_Indices = std::make_unique<uint[]>(m_IndSize);

  bool inverted = false;
  uint n = 0;
  for(uint i = 0; i < m_PosSize; i += 4 * 6) {
    // vec4 of rgba
    for(uint j = 0; j < 4; j++) {
      m_Positions[i + 2 + j * 6] = inverted ? 0.2f : 0.78f;
      m_Positions[i + 3 + j * 6] = inverted ? 0.85f : 0.0f;
      m_Positions[i + 4 + j * 6] = inverted ? 1.0f : 0.22f;
      m_Positions[i + 5 + j * 6] = 1.0f;
    }
    inverted = !inverted;

    // vec2 of positions
    m_Positions[i + 0] = offX;  //            index=0 |_
    m_Positions[i + 1] = offY;
    m_Positions[i + 6] = offX + cellOffset;  //   index=1 _|
    m_Positions[i + 7] = offY;
    m_Positions[i + 12] = offX + cellOffset;  //  index=2 ‾|
    m_Positions[i + 13] = offY + cellOffset;
    m_Positions[i + 18] = offX;  //           index=3 |‾
    m_Positions[i + 19] = offY + cellOffset;

    if(++n >= m_GridN) {
      offX += cellOffset;
      offY = (m_WinWidth <= m_WinHeight) * ((m_WinHeight - m_WinWidth) / 2);
      n = 0;
      if(!(m_GridN & 1))
        inverted = !inverted;
    } else {
      offY += cellOffset;
    }
  }

  uint indStep = 0;
  for(uint i = 0; i < m_IndSize; i += 6) {
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

void TestBatchRender::changePosArray() {
  static std::default_random_engine generator;
  static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

  for(uint i = 0; i < m_PosSize; i += 4 * 6) {
    // vec4 of rgba
    float r = dist(generator);  //, g = dist(generator), b = dist(generator), a = dist(generator);
    bool white = (r > 0.1f);
    for(uint j = 0; j < 4; j++) {
      // m_Positions[i + 2 + j * 6] = r;
      // m_Positions[i + 3 + j * 6] = g;
      // m_Positions[i + 4 + j * 6] = b;
      // m_Positions[i + 5 + j * 6] = a;
      m_Positions[i + 2 + j * 6] = white ? 1.0f : 0.0f;
      m_Positions[i + 3 + j * 6] = white ? 1.0f : 0.0f;
      m_Positions[i + 4 + j * 6] = white ? 1.0f : 0.0f;
      m_Positions[i + 5 + j * 6] = white ? 1.0f : 0.0f;
    }
  }
  m_VB = std::make_unique<GE::VertexBuffer>(m_Positions.get(), m_PosSize * sizeof(float));
  m_VAO->addBuffer(*m_VB, *m_VBL);
}

void TestBatchRender::onRender() {
  // GLCALL(glClearColor(0.67f, 0.85f, 0.89f, 1.0f));
  GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
  // GLCALL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
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

void TestBatchRender::onUpdate(float &deltaTime) {
  deltaTime = deltaTime;
}

void TestBatchRender::onImGuiRender() {
  if(!m_Initiated) {
    ImGui::Text("Enter the size MxN of the Grid:");
    static char mSize[5] = "";
    static char nSize[5] = "";
    ImGui::PushItemWidth(40);
    ImGui::InputTextWithHint("x", std::to_string(m_GridM).c_str(), mSize, 5,
                             ImGuiInputTextFlags_CharsDecimal);
    ImGui::SameLine();
    ImGui::InputTextWithHint("", std::to_string(m_GridN).c_str(), nSize, 5,
                             ImGuiInputTextFlags_CharsDecimal);
    ImGui::PopItemWidth();

    if(ImGui::Button("Confirm")) {
      char empty[] = "";
      if(strcmp(mSize, empty) and strcmp(nSize, empty)) {
        m_GridM = atoi(mSize);
        m_GridN = atoi(nSize);
      }
      this->init();
      m_Initiated = true;
    }
    ImGui::TextDisabled("If either input is left empty,\n the default grid will be initialized.");
  } else {
    if(ImGui::Button("Test")) {
      this->changePosArray();
    }
  }
}

TestBatchRender::TestBatchRender()
    : m_WinWidth{GE::GraphicsEngine::getInstance().getWindowWidth()},
      m_WinHeight{GE::GraphicsEngine::getInstance().getWindowHeight()},
      m_GridM{150},
      m_GridN{150},
      m_Positions{nullptr},
      m_PosSize{0},
      m_Indices{nullptr},
      m_IndSize{0},
      m_Initiated{false},
      m_ProjMatrix{glm::ortho(0.0f, (float)m_WinWidth, 0.0f, (float)m_WinHeight, -1.0f, 1.0f)},
      m_ViewMatrix{glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))} {
  LOG(DEBUG) << "Creating Batch Render Test";
}

TestBatchRender::~TestBatchRender() {}

}  // namespace TEST