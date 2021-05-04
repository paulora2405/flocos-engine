#include "graphics/renderer.hpp"

namespace GE {

void Renderer::draw(const VertexArray& va,
                    const IndexBuffer& ib,
                    const Shader& shader) const {
  shader.bind();
  va.bind();
  ib.bind();
  GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::clear() const {
  const float r = 171.0f / 255.0f;
  const float g = 219.0f / 255.0f;
  const float b = 227.0f / 255.0f;
  GLCALL(glClearColor(r, g, b, 1.0f));
  GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

Renderer::Renderer() {}

Renderer::~Renderer() {}

}  // namespace GE