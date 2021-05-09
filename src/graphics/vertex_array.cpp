#include "graphics/vertex_array.hpp"

#include "graphics/vertex_buffer.hpp"
#include "graphics/vertex_buffer_layout.hpp"
#include "logging/gl_error.hpp"

namespace GE {

void VertexArray::addBuffer(const VertexBuffer& vb,
                            const VertexBufferLayout& layout) {
  this->bind();
  vb.bind();

  const auto& elements = layout.getElements();
  unsigned long offset = 0;  // ulong so it has the same size as void*

  for(unsigned int i = 0; i < elements.size(); i++) {
    const auto& e = elements[i];
    GLCALL(glEnableVertexAttribArray(i));
    GLCALL(glVertexAttribPointer(i, e.count, e.type, e.normalized,
                                 layout.getStride(), (const void*)offset));
    offset += e.count * VertexBufferElement::getSizeOfType(e.type);
  }
}

void VertexArray::bind() const {
  GLCALL(glBindVertexArray(m_RendererID));
}
void VertexArray::unbind() const {
  GLCALL(glBindVertexArray(0));
}

VertexArray::VertexArray() : m_RendererID{0} {
  GLCALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
  GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}

}  // namespace GE