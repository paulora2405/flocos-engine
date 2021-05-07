#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "graphics/graphics_engine.hpp"
#include "logging/gl_error.hpp"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char const* argv[]) {
  /* Logger configuration */
  START_EASYLOGGINGPP(argc, argv);
  el::Configurations conf("configuration/logger-default.conf");
  el::Loggers::reconfigureLogger("default", conf);
  // el::Loggers::reconfigureAllLoggers(conf);

  //
  switch(argc) {
    case 1: {
      GE::GraphicsEngine::getInstance().mainLoop();
      break;
    }

    case 3: {
      unsigned short w = atoi(argv[1]);
      unsigned short h = atoi(argv[2]);
      GE::GraphicsEngine::getInstance(w, h).mainLoop();
      break;
    }

    default: {
      break;
    }
  }
}