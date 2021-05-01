#include "graphics/graphics_engine.hpp"

namespace GE {

void GraphicsEngine::geMainLoop() {
  /* Positions of the vertex */
  float positions[] = {
      -0.5f, -0.5f,  // 0
      0.5f,  -0.5f,  // 1
      0.5f,  0.5f,   // 2
      -0.5f, 0.5f,   // 3
  };
  unsigned int indices[] = {
      0, 1, 2,  // first triangle
      2, 3, 0,  // second triangle
  };

  /* Unique buffer id */
  unsigned int buffer;
  /* Generate one buffer and put its id in the variable */
  glGenBuffers(1, &buffer);
  /* Binding means selecting a buffer */
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  /* Puts data in the buffer */
  glBufferData(
      GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

  /* Enable the attributes */
  glEnableVertexAttribArray(0);
  /* Defines the vertex attributes */
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  unsigned int ibo;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices,
      GL_STATIC_DRAW);

  GE::ShaderProgramSource source =
      this->geParseShader("res/shaders/triangle_vf.shader");

  unsigned int shader =
      this->geCreateShader(source.VertexSource, source.FragmentSource);
  glUseProgram(shader);

  /* Loop until the user closes the window */
  while(!glfwWindowShouldClose(this->m_window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Modern OpenGL drawing */
    GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    /* Swap front and back buffers */
    glfwSwapBuffers(this->m_window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteProgram(shader);
}

ShaderProgramSource GraphicsEngine::geParseShader(const std::string& path) {
  std::ifstream istream(path);
  if(!istream.is_open()) {
    std::cerr << "FALHA AO ABRIR ARQUIVO " << path << std::endl;
    exit(EXIT_FAILURE);
  }

  enum class ShaderType
  {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
  };

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

unsigned int GraphicsEngine::geCompileShader(
    unsigned int type, const std::string& source) {
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
    LOG(ERROR) << "FAILED TO COMPILE "
               << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
               << " SHADER";
    /* Strips the '\n' included in the string */
    message[strlen(message) - 1] = '\0';
    LOG(ERROR) << message;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

unsigned int GraphicsEngine::geCreateShader(
    const std::string& vertexShader, const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();  // GLint is eq to uint

  unsigned int vs = geCompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = geCompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

GLFWwindow* GraphicsEngine::geGetWindow() {
  return m_window;
}

void GraphicsEngine::geClearGlError() {
  while(glGetError() != GL_NO_ERROR) {
  }
}

bool GraphicsEngine::geLogGlCall() {
  while(GLenum error = glGetError()) {
    LOG(ERROR) << "[OPENGL ERROR]:" << error;
    return false;
  }
  return true;
}

GraphicsEngine::~GraphicsEngine() {
  glfwTerminate();
  LOG(INFO) << "Window terminated successfully";
}

GraphicsEngine::GraphicsEngine(
    const unsigned short& width, const unsigned short& height)
    : m_width{width}, m_height{height} {
  LOG(INFO) << "Initializing the GraphicsEngine instance";
  /* Initialize GLFW library */
  if(!glfwInit()) {
    LOG(ERROR) << "GLFW COULD NOT INITIALIZE";
    return;
  }

  /* Create a windowed mode window and its OpenGL context */
  std::string window_name{
      std::to_string(m_width) + "," + std::to_string(m_height)};
  m_window =
      glfwCreateWindow(m_width, m_height, window_name.c_str(), NULL, NULL);
  if(!m_window) {
    glfwTerminate();
    return;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_window);

  /* Initialize GLEW library */
  if(glewInit() != GLEW_OK) {
    LOG(ERROR) << "ERROR: GLEW COULD NOT INITIALIZE";
    return;
  }

  /* Print OpenGL Version */
  LOG(INFO) << "OpenGL Version: " << glGetString(GL_VERSION);

  /* Window initiated successfully */
  LOG(INFO) << "Window initiated successfully";
}

}  // namespace GE