#ifndef GRAPHICS_ENGINE_HPP
#define GRAPHICS_ENGINE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <iostream>
#include <string>

namespace GE {
struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

class GraphicsEngine {
private:
  GLFWwindow* window;
  unsigned short WIDTH;
  unsigned short HEIGHT;

public:
  GraphicsEngine(unsigned short width, unsigned short height);
  ~GraphicsEngine();
};

}  // namespace GE

#endif