#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logging/easylogging++.h"

#define ASSERT(x) \
  if(!(x)) raise(SIGTRAP);
#define GLCALL(x)     \
  GE::ClearGlError(); \
  x;                  \
  ASSERT(GE::LogGlCall());

namespace GE {

/*!
 * @brief Calls glGetError() until there are no errors.
 */
void ClearGlError();

/*!
 * @brief Checks for errors.
 * @returns False if errors have been found. Otherwise True.
 */
bool LogGlCall();

}  // namespace GE

#endif