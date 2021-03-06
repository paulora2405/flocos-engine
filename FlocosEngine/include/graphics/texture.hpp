#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

namespace GE {

class Texture {
private:
  unsigned int m_RendererID;
  std::string m_Filepath;
  unsigned char* m_LocalBuffer;
  int m_Width, m_Height, m_BPP;

public:
  /*! @brief Contructs a Texture instance.
   * @param[in] filepath Path to texture file.
   */
  Texture(const std::string& filepath);
  ~Texture();

  void bind(unsigned int slot = 0) const;
  void unbind() const;

  inline uint getTexID() const { return m_RendererID; }
  inline int getWidth() const { return m_Width; }
  inline int getHeight() const { return m_Height; }
};

}  // namespace GE

#endif