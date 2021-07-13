#include "graphics/graphics_engine.hpp"

int main(int argc, char const* argv[]) {
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
      std::cerr << "ARGUMENT ERROR: 0 or 2 Arguments Possible: Width, Height." << std::endl;
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}