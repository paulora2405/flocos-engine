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

#include "graphics/index_buffer.hpp"
#include "graphics/renderer.hpp"
#include "graphics/vertex_array.hpp"
#include "graphics/vertex_buffer.hpp"
#include "logging/easylogging++.h"

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
   * @brief Defines width and height members with parameters and calls geInit().
   * @param[in] width Width of the window (defaults to 800).
   * @param[in] height Height of the window (defaults to 640).
   */
  GraphicsEngine(
      const unsigned short& width = 800, const unsigned short& height = 640);
  /*!
   * @brief Terminates the GLFW Window.
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
  GLFWwindow* geGetWindow() const;

  // TODO
  void geMainLoop();

private:
  /*!
   * @brief Initializes GLFW, creates a Window, makes window the current
   * context, and then initializes GLEW.
   */
  void geInit();

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