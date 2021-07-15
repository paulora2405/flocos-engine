#ifndef GRAPHICS_ENGINE_HPP
#define GRAPHICS_ENGINE_HPP

#include "logging/gl_error.hpp"

namespace GE {

/// @brief Singleton GraphicsEngine Class.
class GraphicsEngine {
private:
  GLFWwindow* m_window;
  const unsigned short m_width;
  const unsigned short m_height;

private:
  /*! @brief Defines width and height members with parameters and calls
   * init().
   * @param[in] width Width of the window.
   * @param[in] height Height of the window.
   */
  GraphicsEngine(const unsigned short& width, const unsigned short& height);

  /// @brief Terminates the GLFW Window.
  ~GraphicsEngine();

  /*! @brief Initializes GLFW, creates a Window, makes window the current
   * context, and then initializes GLEW.
   */
  void init();

  /// @brief Calls GLFW SwapBuffers and PollEvents functions.
  void swapAndPoll();

public:
  /*! @brief Constructs a instance of Class if not already constructed.
   * @param[in] width Width of the window (defaults to 800).
   * @param[in] height Height of the window (defaults to 640).
   * @returns A reference to the single Class instance.
   */
  static GraphicsEngine& getInstance(const unsigned short& width = 1600,
                                     const unsigned short& height = 900);

  /// @brief The copy constructor is forbidden.
  GraphicsEngine(const GraphicsEngine&) = delete;
  /// @brief The assignment operator is forbidden.
  GraphicsEngine& operator=(const GraphicsEngine&) = delete;

  /// @returns Pointer to the window's instance.
  GLFWwindow* getWindow() const;

  unsigned short getWindowWidth() const;
  unsigned short getWindowHeight() const;

  /// @brief Executes main OpenGL loop.
  void mainLoop();
};

}  // namespace GE

#endif