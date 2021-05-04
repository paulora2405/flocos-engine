#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "graphics/vertex_buffer.hpp"
#include "graphics/vertex_buffer_layout.hpp"
#include "logging/gl_error.hpp"

namespace GE {

class VertexArray {
private:
  unsigned int m_RendererID;

public:
  VertexArray();
  ~VertexArray();

  void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
  void bind() const;
  void unbind() const;
};

}  // namespace GE

#endif