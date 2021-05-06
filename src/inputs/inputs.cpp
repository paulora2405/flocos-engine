#include "inputs/inputs.hpp"

namespace inputs {

void handleKeyboard(
    GLFWwindow* window, int key, int scancode, int action, int mods) {
  scancode += 0;
  mods += 0;

  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

}  // namespace inputs