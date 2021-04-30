#ifndef GRAPHICS_ENGINE_HPP
#define GRAPHICS_ENGINE_HPP
// -*- lsst-c++ -*-

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/*!
 * @brief Graphics Engine Namespace
 */
namespace GE {
struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

class GraphicsEngine {
private:
  GLFWwindow* m_window;
  const unsigned short m_width;
  const unsigned short m_height;

public:
  /*!
   * @brief Initializes GLFW, creates a Window, makes window the current
   * context, and then initializes GLEW.
   * @param[in] width Width of the window.
   * @param[in] height Height of the window.
   */
  GraphicsEngine(unsigned short width, unsigned short height);
  /*!
   */
  ~GraphicsEngine();

  /*!
   * @brief The copy constructor is forbidden.
   */
  GraphicsEngine(const GraphicsEngine&) = delete;

  /*!
   * @brief The assignment operator is forbidden.
   */
  GraphicsEngine& operator=(const GraphicsEngine&) = delete;

  /*!
   * @returns Pointer to the window's instance.
   */
  GLFWwindow* geGetWindow();

  // TODO
  void geMainLoop();

  // private:
  /*!
   * @brief Parses a dual-shader file (vertex and fragment shader in the same
   * file) to a ShaderProgramSource struct.
   * @param[in] path Path to the shader file.
   * @returns A ShaderProgramSource struct.
   */
  ShaderProgramSource geParseShader(const std::string& path);

  /*!
   * @brief Compiles a Shader with a specific type from its source code.
   * @param[in] type GLEW Shader type, e.g. GL_VERTEX_SHADER or
   * GL_FRAGMENT_SHADER.
   * @returns The program id.
   */
  unsigned int geCompileShader(unsigned int type, const std::string& source);

  /*!
   * @brief Creates a program and attaches the vertex and fragment shader to it.
   * @param[in] vertexShader Vertex Shader source code.
   * @param[in] fragmentShader Fragment Shader source code.
   * @returns The program id.
   */
  unsigned int geCreateShader(
      const std::string& vertexShader, const std::string& fragmentShader);
};

}  // namespace GE

#endif