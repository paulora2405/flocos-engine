#include "graphics/gui.hpp"

namespace GE {

void Gui::newFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Gui::draw() {
  if(m_ShowDemoWindow) ImGui::ShowDemoWindow(&m_ShowDemoWindow);

  this->drawFps();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::drawSliders(const glm::vec3& t, glm::vec2 screen) {
  ImGui::Begin("Picture Position");

  ImGui::SliderFloat("X", (float*)&t.x, 0.0f, (float)screen.x);
  ImGui::SliderFloat("Y", (float*)&t.y, 0.0f, (float)screen.y);

  ImGui::End();
}

void Gui::drawFps() {
  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  // window_flags |= ImGuiWindowFlags_NoBackground;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoScrollbar;
  window_flags |= ImGuiWindowFlags_NoInputs;
  window_flags |= ImGuiWindowFlags_NoDecoration;

  ImGui::Begin("FPS", nullptr, window_flags);
  ImGui::Text("%.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate,
              1000.0f / ImGui::GetIO().Framerate);
  ImGui::End();
}

Gui::Gui(GLFWwindow* window, bool drawDemo) : m_ShowDemoWindow{drawDemo} {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui::StyleColorsDark();
  ImGui_ImplOpenGL3_Init((char*)glGetString(330));
}

Gui::~Gui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

}  // namespace GE