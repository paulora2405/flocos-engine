#ifndef GL_ERROR_HPP
#define GL_ERROR_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/easylogging/easylogging++.hpp"

#define ASSERT(x) \
  if(!(x)) raise(SIGTRAP);
#define GLCALL(x)     \
  GE::clearGlError(); \
  x;                  \
  ASSERT(GE::logGlCall());

namespace GE {

/// @brief Calls glGetError() until there are no errors.
void clearGlError();

/*! @brief Checks for errors.
 * @returns False if errors have been found. Otherwise True.
 */
bool logGlCall();

}  // namespace GE

#endif