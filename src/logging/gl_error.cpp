#include "logging/gl_error.hpp"

namespace GE {

void clearGlError() {
  while(glGetError() != GL_NO_ERROR) {
  }
}

bool logGlCall() {
  while(GLenum error = glGetError()) {
    LOG(ERROR) << "[OPENGL ERROR]:" << error;
    return false;
  }
  return true;
}

}