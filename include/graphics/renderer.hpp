#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "vendor/glm/glm.hpp"

namespace GE {

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer {
private:
public:
  Renderer();
  ~Renderer();

  /*! @brief Executes a draw call.
   * @param[in] va Vertex Array.
   * @param[in] ib Index Buffer.
   * @param[in] shader Shader.
   */
  void draw(const GE::VertexArray& va,
            const GE::IndexBuffer& ib,
            const GE::Shader& shader) const;

  /// @brief Clears OpenGL draw buffers.
  void clear(const glm::vec4 color = {0.67f, 0.85f, 0.89f, 1.0f}) const;
};

}  // namespace GE

#endif