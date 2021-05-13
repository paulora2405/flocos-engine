#include "graphics/texture.hpp"

#include "logging/gl_error.hpp"
#include "vendor/stbi/stb_image.hpp"

namespace GE {

void Texture::bind(unsigned int slot /* = 0 */) const {
  GLCALL(glActiveTexture(GL_TEXTURE0 + slot));

  GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::unbind() const {
  GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::Texture(const std::string& filepath)
    : m_RendererID{0},
      m_Filepath{filepath},
      m_LocalBuffer{nullptr},
      m_Width{0},
      m_Height{0},
      m_BPP{0} {
  stbi_set_flip_vertically_on_load(1);
  m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

  GLCALL(glGenTextures(1, &m_RendererID));
  GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

  GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA,
                      GL_UNSIGNED_BYTE, m_LocalBuffer));
  GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

  if(m_LocalBuffer) stbi_image_free(m_LocalBuffer);
}

Texture::~Texture() {
  GLCALL(glDeleteTextures(1, &m_RendererID));
}

}  // namespace GE