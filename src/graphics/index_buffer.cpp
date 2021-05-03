#include "graphics/index_buffer.hpp"

namespace GE {

void IndexBuffer::bind() const {
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind() const {
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count{count} {
  ASSERT(sizeof(unsigned int) == sizeof(GLuint));

  /* Generate one buffer and put its id in the variable */
  GLCALL(glGenBuffers(1, &m_RendererID));
  /* Binding means selecting a buffer */
  GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
  /* Puts data in the buffer */
  GLCALL(glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data,
      GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
  GLCALL(glDeleteBuffers(1, &m_RendererID));
}

}  // namespace GE