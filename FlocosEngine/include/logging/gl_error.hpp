#ifndef GL_ERROR_HPP
#define GL_ERROR_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/easylogging/easylogging++.hpp"

#define ELPP_NO_DEFAULT_LOG_FILE

#if defined(NDEBUG)
  #define GLCALL(x) x
  #define ASSERT(x)
  #define ELPP_DISABLE_DEBUG_LOGS

#else
  #include <signal.h>
  #define ASSERT(x) \
    if(!(x)) raise(SIGTRAP)

  #define GLCALL(x)     \
    GE::clearGlError(); \
    x;                  \
    ASSERT(GE::logGlCall())
#endif

namespace GE {

/// @brief Calls glGetError() until there are no errors.
void clearGlError();

/*! @brief Checks for errors.
 * @returns False if errors have been found. Otherwise True.
 */
bool logGlCall();

}  // namespace GE

#endif