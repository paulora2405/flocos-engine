#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

namespace GE {

class VertexBuffer {
private:
  unsigned int m_RendererID;

public:
  /*! @brief Creates vertex buffer.
   * @param[in] data Pointer to the data.
   * @param[in] size Size in bytes of the data.
   */
  VertexBuffer(const void* data, unsigned int size);
  ~VertexBuffer();

  void bind() const;
  void unbind() const;
};

}  // namespace GE

#endif