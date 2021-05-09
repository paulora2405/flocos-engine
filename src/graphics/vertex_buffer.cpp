#include "graphics/vertex_buffer.hpp"

#include "logging/gl_error.hpp"

namespace GE {

void VertexBuffer::bind() const {
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unbind() const {
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    : m_RendererID{0} {
  /* Generate one buffer and put its id in the variable */
  GLCALL(glGenBuffers(1, &m_RendererID));
  /* Binding means selecting a buffer */
  GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  /* Puts data in the buffer */
  GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
  GLCALL(glDeleteBuffers(1, &m_RendererID));
}

}  // namespace GE