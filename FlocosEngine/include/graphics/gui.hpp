#ifndef GUI_HPP
#define GUI_HPP

#include <string>

#include "logging/gl_error.hpp"
#include "vendor/glm/glm.hpp"

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
                   glm::vec2 screen) const;

private:
  /// @brief Called in the draw() function every ImGUI draw call.
  void drawFps() const;
};

}  // namespace GE

#endif