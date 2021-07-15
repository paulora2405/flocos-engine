#include "graphics/shader.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "logging/gl_error.hpp"

namespace GE {

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
  GLCALL(glUniform4f(this->getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniform1f(const std::string& name, float value) {
  GLCALL(glUniform1f(this->getUniformLocation(name), value));
}

void Shader::setUniform1i(const std::string& name, int value) {
  GLCALL(glUniform1i(this->getUniformLocation(name), value));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
  GLCALL(glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string& name) const {
  if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  // LOG(DEBUG) << "Uniform " << name << " not found on cache";

  GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if(location == -1)
    LOG(WARNING) << "Uniform '" << name << "' could not be located!";

  m_UniformLocationCache[name] = location;

  return location;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath) {
  std::ifstream istream(filepath);
  if(!istream.is_open()) {
    std::cerr << "FALHA AO ABRIR ARQUIVO " << filepath << std::endl;
    exit(EXIT_FAILURE);
  }

  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while(getline(istream, line)) {
    if(line.find("#shader") != std::string::npos) {
      if(line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;

      else if(line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    } else {
      ss[(int)type] << line << '\n';
    }
  }

  return {ss[0].str(), ss[1].str()};
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    LOG(ERROR) << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
               << " SHADER";
    /* Strips the '\n' included in the string */
    message[strlen(message) - 1] = '\0';
    LOG(ERROR) << message;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

unsigned int Shader::createShader(const std::string& vertexShader,
                                  const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();  // GLint is eq to uint

  unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

void Shader::bind() const {
  GLCALL(glUseProgram(m_RendererID));
}

void Shader::unbind() const {
  GLCALL(glUseProgram(0));
}

Shader::Shader(const std::string& filepath) : m_Filepath{filepath}, m_RendererID{0} {
  // LOG(DEBUG) << "Parsing and compiling shader on " << filepath;
  ShaderProgramSource source = this->parseShader(filepath);
  m_RendererID = this->createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
  GLCALL(glDeleteProgram(m_RendererID));
}

}  // namespace GE