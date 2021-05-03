#include "graphics/renderer.hpp"
namespace GE {

void ClearGlError() {
  while(glGetError() != GL_NO_ERROR) {
  }
}

bool LogGlCall() {
  while(GLenum error = glGetError()) {
    LOG(ERROR) << "[OPENGL ERROR]:" << error;
    return false;
  }
  return true;
}

}  // namespace GE