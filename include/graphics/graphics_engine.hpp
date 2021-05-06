#ifndef GRAPHICS_ENGINE_HPP
#define GRAPHICS_ENGINE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <signal.h>
#include <stdlib.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "graphics/gui.hpp"
#include "graphics/index_buffer.hpp"
#include "graphics/renderer.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"
#include "graphics/vertex_buffer.hpp"
#include "graphics/vertex_buffer_layout.hpp"
#include "inputs/inputs.hpp"
#include "logging/gl_error.hpp"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

namespace GE {

class GraphicsEngine {
private:
  GLFWwindow* m_window;
  const unsigned short m_width;
  const unsigned short m_height;

public:
  /*! @brief Defines width and height members with parameters and calls
   * init().
   * @param[in] width Width of the window (defaults to 800).
   * @param[in] height Height of the window (defaults to 640).
   */
  GraphicsEngine(const unsigned short& width = 800,
                 const unsigned short& height = 640);

  /// @brief Terminates the GLFW Window.
  ~GraphicsEngine();

  /// @brief The copy constructor is forbidden.
  GraphicsEngine(const GraphicsEngine&) = delete;

  /// @brief The assignment operator is forbidden.
  GraphicsEngine& operator=(const GraphicsEngine&) = delete;

  /// @returns Pointer to the window's instance.
  GLFWwindow* getWindow() const;

  /// @brief Executes main OpenGL loop.
  void mainLoop();

private:
  /*! @brief Initializes GLFW, creates a Window, makes window the current
   * context, and then initializes GLEW.
   */
  void init();

  /// @brief Calls GLFW SwapBuffers and PollEvents functions.
  void swapAndPoll();
};

}  // namespace GE

#endif