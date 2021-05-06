#ifndef VERTEX_BUFFER_LAYOUT_HPP
#define VERTEX_BUFFER_LAYOUT_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "logging/gl_error.hpp"

namespace GE {

struct VertexBufferElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int getSizeOfType(unsigned int type) {
    switch(type) {
      case GL_FLOAT:
        return sizeof(GLfloat);
      case GL_UNSIGNED_INT:
        return sizeof(GLuint);
      case GL_UNSIGNED_BYTE:
        return sizeof(GLbyte);
    }
    LOG(ERROR) << "VertexBufferElement type error (type passed:" << type << ")";
    ASSERT(false);
    return 0;
  }
};

class VertexBufferLayout {
private:
  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;

public:
  VertexBufferLayout() : m_Stride{0} {}

  /// @brief Only types allowed are float, unsigned int and unsigned char.
  template <typename T>
  void push(unsigned int count);

  inline const std::vector<VertexBufferElement>& getElements() const {
    return m_Elements;
  }

  inline unsigned int getStride() const { return m_Stride; }
};

}  // namespace GE

#endif