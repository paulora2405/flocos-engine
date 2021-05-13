#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

namespace GE {

class IndexBuffer {
private:
  unsigned int m_RendererID;
  unsigned int m_Count;

public:
  /*! @brief Creates index buffer.
   * @param[in] data Pointer to the data.
   * @param[in] count Quantity of indexes.
   */
  IndexBuffer(const unsigned int* data, unsigned int count);
  ~IndexBuffer();

  void bind() const;
  void unbind() const;

  inline unsigned int GetCount() const { return m_Count; }
};

}  // namespace GE

#endif