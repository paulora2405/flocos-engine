#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

namespace GE {

class VertexBuffer;
class VertexBufferLayout;

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