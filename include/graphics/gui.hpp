#ifndef GUI_HPP
#define GUI_HPP

#include <string>

#include "logging/gl_error.hpp"
#include "vendor/glm/glm.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

namespace GE {

class Gui {
private:
  bool m_ShowDemoWindow;

public:
  /*! @brief Initializes ImGUI.
   * @param[in] window Pointer to GLFW window.
   */
  Gui(GLFWwindow* window, bool drawDemo = false);

  /// @brief Terminates ImGUI.
  ~Gui();

  /// @brief Creates new ImGUI Frame.
  void newFrame();

  /// @brief ImGUI draw call.
  void draw();

  void drawSliders(const std::string& label,
                   const glm::vec3& t,
                   glm::vec2 screen);

private:
  /// @brief Called in the draw() function every ImGUI draw call.
  void drawFps();
};

}  // namespace GE

#endif