#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <unordered_map>

#include "vendor/glm/glm.hpp"

namespace GE {

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader {
private:
  std::string m_Filepath;
  unsigned int m_RendererID;
  mutable std::unordered_map<std::string, int> m_UniformLocationCache;

public:
  /*! @brief Shader constructor.
   * @param[in] filepath Path to the shader file.
   */
  Shader(const std::string& filepath);

  /// @brief Deletes the the shader.
  ~Shader();

  void bind() const;
  void unbind() const;

  /*! @brief Sets the values of a 4-floats uniform.
   * @param[in] name Name of the uniform to set.
   * @param[in] v0 Value 0.
   * @param[in] v1 Value 1.
   * @param[in] v2 Value 2.
   * @param[in] v3 Value 3.
   */
  void setUniform4f(
      const std::string& name, float v0, float v1, float v2, float v3);
  /*! @brief Sets the value of a 1-float uniform.
   * @param[in] name Name of the uniform to set.
   * @param[in] value Value.
   */
  void setUniform1f(const std::string& name, float value);
  /*! @brief Sets the value of a 1-int uniform.
   * @param[in] name Name of the uniform to set.
   * @param[in] value Value.
   */
  void setUniform1i(const std::string& name, int value);

  /*! @brief Sets the value of a 4-float matrix uniform.
   * @param[in] name Name of the uniform to set.
   * @param[in] matrix Mat4 to set.
   */
  void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
  /*! @brief Locates a uniform inside the shader.
   * @param[in] name Name of the uniform.
   * @returns The uniforms unique id.
   */
  int getUniformLocation(const std::string& name) const;

  /*! @brief Parses a dual-shader file (vertex and fragment shader in the same
   * file) to a ShaderProgramSource struct.
   * @param[in] filepath Path to the shader file.
   * @returns A ShaderProgramSource struct.
   */
  ShaderProgramSource parseShader(const std::string& filepath);

  /*! @brief Compiles a Shader with a specific type from its source code.
   * @param[in] type GLEW Shader type, e.g. GL_VERTEX_SHADER or
   * GL_FRAGMENT_SHADER.
   * @param[in] source Parsed source code of the shader.
   * @returns The program id.
   */
  unsigned int compileShader(unsigned int type, const std::string& source);

  /*! @brief Creates a program and attaches the vertex and fragment shader to
   * it.
   * @param[in] vertexShader Vertex Shader source code.
   * @param[in] fragmentShader Fragment Shader source code.
   * @returns The program id.
   */
  unsigned int createShader(const std::string& vertexShader,
                            const std::string& fragmentShader);
};

}  // namespace GE

#endif