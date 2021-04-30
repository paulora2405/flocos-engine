#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "graphics/graphics_engine.hpp"

int main(int argc, char const* argv[]) {
  unsigned short WIDTH;
  unsigned short HEIGHT;
  if(argc == 1)
    WIDTH = 800, HEIGHT = 640;
  else if(argc == 3)
    WIDTH = atoi(argv[1]), HEIGHT = atoi(argv[2]);
  else {
    std::cerr << "ARGUMENT ERROR\n";
    return EXIT_FAILURE;
  }

  GE::GraphicsEngine ge(WIDTH, HEIGHT);

  ge.geMainLoop();
}