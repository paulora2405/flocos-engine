#include "tests/test_ants_heterodata.hpp"

#include <fstream>
#include <random>
#include <set>
#include <string>

#include "graphics/graphics_engine.hpp"
#include "graphics/renderer.hpp"
#include "logging/gl_error.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"

namespace TEST {

std::vector<std::vector<float>> TestAntsHeterodata::inputData(std::filesystem::path filepath) {
  std::vector<std::vector<float>> dataWithGroup;

  std::ifstream source("res/inputs/" + std::string(filepath.empty() ? "4_groups.txt" : filepath));

  if(source.is_open()) {
    for(std::string line; std::getline(source, line);) {
      std::istringstream in(line);

      float x, y, z;
      in >> x >> y >> z;
      dataWithGroup.push_back({x, y, z});
    }
  } else
    LOG(ERROR) << "Could not open " << filepath;

  m_DeadQnt = dataWithGroup.size();

  return dataWithGroup;
}

void TestAntsHeterodata::saveGridToFile() {
  auto gridState = m_Colony->getGridState();
  std::string filename{"grid" + std::to_string(m_GridM) + 'x' + std::to_string(m_GridN) + ".pgm"};
  std::ofstream os(filename);
  if(os.is_open()) {
    os << "P2\n" << m_GridM << ' ' << m_GridN << " 1\n";
    for(int j = m_GridN - 1; j >= 0; --j) {
      for(int i = 0; i < m_GridM; ++i)
        os << (gridState[i * m_GridM + j] ? "0" : "1") << '\n';
    }
  } else
    LOG(ERROR) << "Couldn't open " << filename << "!";
}

void TestAntsHeterodata::init(std::vector<std::vector<float>> dataWithGroup) {
  LOG(INFO) << "Initializing Ants Simulation Test";
  LOG(DEBUG) << "Initializing Ants Simulation Test";
  float offX = (m_WinHeight < m_WinWidth) * ((m_WinWidth - m_WinHeight) / 2);
  float offY = (m_WinWidth <= m_WinHeight) * ((m_WinHeight - m_WinWidth) / 2);
  float cellOffset = (m_WinWidth <= m_WinHeight) * ((float)m_WinWidth / (float)m_GridM) +
                     (m_WinHeight < m_WinWidth) * ((float)m_WinHeight / (float)m_GridN);

  uint cellsTotal = m_GridM * m_GridN;
  m_PosSize = cellsTotal * 4 * 6;
  m_IndSize = cellsTotal * 6;

  std::set<uint8_t> groups;
  for(size_t i = 0; i < dataWithGroup.size(); i++)
    if(groups.count(static_cast<uint8_t>(dataWithGroup.at(i).at(2))) == 0)
      groups.insert(static_cast<uint8_t>(dataWithGroup.at(i).at(2)));

  std::default_random_engine gen;
  std::uniform_real_distribution<float> dist(0.3, 0.9);

  m_GroupsColors.resize(groups.size());

  for(auto group : groups) {
    m_GroupsColors.at(group - 1) = {dist(gen), dist(gen), dist(gen)};
  }

  m_Colony = std::make_unique<SIM_HETERODATA::Colony>(m_GridM, m_GridN, m_AliveQnt, m_DeadQnt,
                                                      m_VisionRadius, dataWithGroup);

  m_Positions = std::make_unique<float[]>(m_PosSize);
  m_Indices = std::make_unique<uint[]>(m_IndSize);

  uint n = 0, m = 0;
  for(uint i = 0; i < m_PosSize; i += 4 * 6) {
    // vec4 of rgba
    SIM_HETERODATA::GridState state = m_Colony.get()->query(m, n);
    float r = 0.0f, g = 0.0f, b = 1.0f, a = 1.0f;

    if(state == SIM_HETERODATA::GridState::Empty)
      r = g = b = a = 1.0f;
    else if(state == SIM_HETERODATA::GridState::DeadFree) {
      uint8_t group = m_Colony->getGroupAt(m, n);
      r = m_GroupsColors[group - 1][0];
      g = m_GroupsColors[group - 1][1];
      b = m_GroupsColors[group - 1][2];
      a = 1.0f;
    } else if(state == SIM_HETERODATA::GridState::AliveFree)
      r = 0.0f, g = 1.0f, b = 0.0f, a = 1.0f;
    else if(state == SIM_HETERODATA::GridState::BothFree)
      r = 1.0f, g = 1.0f, b = 0.0f, a = 1.0f;
    else if(state == SIM_HETERODATA::GridState::AliveBusy)
      r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;

    for(u_char j = 0; j < 4; ++j) {
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
      n = 0, ++m;
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

//

void TestAntsHeterodata::updateGrid() {
  uint n = 0, m = 0;
  for(uint i = 0; i < m_PosSize; i += 4 * 6) {
    // vec4 of rgba
    SIM_HETERODATA::GridState state = m_Colony->query(m, n);
    float r = 0.0f, g = 0.0f, b = 1.0f, a = 1.0f;

    if(state == SIM_HETERODATA::GridState::Empty)
      r = g = b = a = 1.0f;
    else if(state == SIM_HETERODATA::GridState::DeadFree) {
      uint8_t group = m_Colony->getGroupAt(m, n);
      r = m_GroupsColors[group - 1][0];
      g = m_GroupsColors[group - 1][1];
      b = m_GroupsColors[group - 1][2];
      a = 1.0f;
    } else if(!m_HideAlive) {
      if(state == SIM_HETERODATA::GridState::AliveFree)
        r = 0.0f, g = 1.0f, b = 0.0f, a = 1.0f;
      if(state == SIM_HETERODATA::GridState::AliveBusy)
        r = 1.0f, g = 0.0f, b = 0.0f, a = 1.0f;
      if(state == SIM_HETERODATA::GridState::AliveBusyDeadFree)
        r = 1.0f, g = 0.4f, b = 0.0f, a = 1.0f;
      if(state == SIM_HETERODATA::GridState::BothFree)
        r = 1.0f, g = 1.0f, b = 0.0f, a = 1.0f;
    } else if(state == SIM_HETERODATA::GridState::AliveBusy or
              state == SIM_HETERODATA::GridState::AliveBusyDeadFree or
              state == SIM_HETERODATA::GridState::BothFree) {
      uint8_t group = m_Colony->getGroupAt(m, n);
      r = m_GroupsColors[group - 1][0];
      g = m_GroupsColors[group - 1][1];
      b = m_GroupsColors[group - 1][2];
      a = 1.0f;
    } else
      r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;

    for(u_char j = 0; j < 4; ++j) {
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

void TestAntsHeterodata::onUpdate(float& deltaTime) {
  deltaTime = deltaTime;
  static uint tick = 0;
  if(!m_Paused and m_Initiated and !m_Finished) {
    if(++tick >= 8)
      updateGrid(), tick = 0;

    m_Colony->iterate();
    ++m_Iterations;
  }
}

//

void TestAntsHeterodata::onRender() {
  GLCALL(glClearColor(0.85f, 0.96f, 0.65f, 1.0f));
  GLCALL(glClear(GL_COLOR_BUFFER_BIT));
  if(m_Initiated) {
    GE::Renderer re;

    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), {0, 0, 0});
    /* P * V * M because opengl uses coloum-major matrices */
    glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model_matrix;

    m_Shader->bind();
    m_Shader->setUniformMat4f("u_MVP", mvp);
    re.draw(*m_VAO, *m_IB, *m_Shader);
  }
}

//

void TestAntsHeterodata::onImGuiRender() {
  if(!m_Initiated) {
    static char filepath[64] = "";
    ImGui::InputTextWithHint("Filepath", "4_groups.txt", filepath, 64);
    if(ImGui::Button("Input Data")) {
      m_Initiated = true;
      this->init(this->inputData(filepath));
    }

    ImGui::Text("Enter the size MxN of the Grid:");
    if(ImGui::IsItemHovered())
      ImGui::SetTooltip("If either input is left empty,\nthe default grid will be initialized.");
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
        m_AliveQnt =
            strcmp(aliveAntsQnt, empty) ? atoi(aliveAntsQnt) : (m_GridM * m_GridN) / 64 + 1;
        m_DeadQnt = strcmp(deadAntsQnt, empty) ? atoi(deadAntsQnt) : (m_GridM * m_GridN) / 8 + 1;
        m_VisionRadius = strcmp(antsVisionRadius, empty) ? atoi(antsVisionRadius) : 3;
      }
      m_Initiated = true;
      this->init({});
    }
  } else {
    ImGui::Text(std::string("Running " + std::to_string(m_GridM) + 'x' + std::to_string(m_GridN) +
                            " Grid...")
                    .c_str());
    if(ImGui::IsItemHovered())
      ImGui::SetTooltip(std::string("Alive Ants: " + std::to_string(m_AliveQnt) +
                                    "\nDead Ants: " + std::to_string(m_DeadQnt) +
                                    "\nVision Radius: " + std::to_string(m_VisionRadius) +
                                    "\nIterations: " + std::to_string(m_Iterations))
                            .c_str());

    static std::string pauseText{"Start"};
    if(!m_Finished and ImGui::Button(pauseText.c_str())) {
      m_Paused = !m_Paused;
      if(m_Paused)
        pauseText = "Continue";
      else
        pauseText = "Pause";
    }
    if(!m_Finished and ImGui::Button("Hide/Show AliveAnts"))
      m_HideAlive = !m_HideAlive;
    if(!m_Finished and m_Paused)
      if(ImGui::Button("Update One Tick")) {
        m_Colony->iterate();
        ++m_Iterations;
        this->updateGrid();
      }
    if(!m_Finished and ImGui::Button("FastForward 10000 Iterations")) {
      for(uint i = 0; i < 10000; ++i) {
        m_Colony->iterate();
        ++m_Iterations;
      }
    }
    if(!m_Finishing) {
      if(ImGui::Button("Finish")) {
        m_Colony->endIterations();
        m_Finishing = true;
      }
    } else if(m_Finished or m_Colony->hasFinished()) {
      m_Finished = true;
      this->updateGrid();
      if(ImGui::Button("Save Current State to File")) {
        this->saveGridToFile();
      }
    }
  }
}

//

TestAntsHeterodata::TestAntsHeterodata()
    : m_WinWidth{GE::GraphicsEngine::getInstance().getWindowWidth()},
      m_WinHeight{GE::GraphicsEngine::getInstance().getWindowHeight()},
      m_GridM{50},
      m_GridN{50},
      m_AliveQnt{(m_GridM * m_GridN) / (uint)64 + 1},
      m_DeadQnt{(m_GridM * m_GridN) / (uint)8 + 1},
      m_VisionRadius{1},
      m_Iterations{0},
      m_Positions{},
      m_PosSize{0},
      m_Indices{},
      m_IndSize{0},
      m_GroupsColors{},
      m_Initiated{false},
      m_Paused{true},
      m_HideAlive{false},
      m_Finishing{false},
      m_Finished{false},
      m_ProjMatrix{glm::ortho(0.0f, (float)m_WinWidth, 0.0f, (float)m_WinHeight, -1.0f, 1.0f)},
      m_ViewMatrix{glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))},
      m_Colony{} {
  LOG(DEBUG) << "Creating Ants Test";
}

//

TestAntsHeterodata::~TestAntsHeterodata() {}

}  // namespace TEST