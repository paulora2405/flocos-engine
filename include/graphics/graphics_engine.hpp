#ifndef GRAPHICS_ENGINE_HPP
#define GRAPHICS_ENGINE_HPP

#include "logging/gl_error.hpp"

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