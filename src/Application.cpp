#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "graphics/graphics_engine.hpp"
#include "logging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char const* argv[]) {
  /* Logger configuration */
  START_EASYLOGGINGPP(argc, argv);
  el::Configurations conf("configuration/logger-default.conf");
  el::Loggers::reconfigureLogger("default", conf);
  // el::Loggers::reconfigureAllLoggers(conf);

  //
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