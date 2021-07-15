#ifndef INPUTS_HPP
#define INPUTS_HPP

#include "logging/gl_error.hpp"

namespace inputs {

/// @brief Key press handling.
void handleKeyboard(
    GLFWwindow* window, int key, int scancode, int action, int mods);

}  // namespace inputs

#endif