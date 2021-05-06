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

void Renderer::clear(const glm::vec4 c) const {
  GLCALL(glClearColor(c.r, c.g, c.b, c.a));
  GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

Renderer::Renderer() {}

Renderer::~Renderer() {}

}  // namespace GE