#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

short WIDTH;
short HEIGHT;

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& file_name) {
  std::ifstream istream(file_name);
  if(!istream.is_open()) {
    std::cerr << "FALHA AO ABRIR ARQUIVO " << file_name << std::endl;
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

static unsigned int CompileShader(unsigned int type, const std::string& source) {
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
    std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
              << "shader" << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string& vertexShader,
                                 const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();  // GLint is eq to uint

  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main(int argc, char const* argv[]) {
  if(argc == 1)
    WIDTH = 800, HEIGHT = 640;
  else if(argc == 3)
    WIDTH = atoi(argv[1]), HEIGHT = atoi(argv[2]);
  else {
    std::cerr << "ARGUMENT ERROR\n";
    return EXIT_FAILURE;
  }

  GLFWwindow* window;

  /* Initialize GLFW library */
  if(!glfwInit()) {
    std::cerr << "GLEW COULD NOT INITIALIZE\n";
    return EXIT_FAILURE;
  }

  /* Create a windowed mode window and its OpenGL context */
  std::string window_name{std::to_string(WIDTH) + "," + std::to_string(HEIGHT)};
  window = glfwCreateWindow(WIDTH, HEIGHT, window_name.c_str(), NULL, NULL);
  if(!window) {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Initialize GLEW library */
  if(glewInit() != GLEW_OK) {
    std::cerr << "GLEW COULD NOT INITIALIZE\n";
    return EXIT_FAILURE;
  }

  /* Print OpenGL Version */
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  /* Unique buffer id */
  unsigned int buffer;
  /* Positions of the vertex */
  float positions[] = {-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f};
  /* Generate one buffer and put its id in the variable */
  glGenBuffers(1, &buffer);
  /* Binding means selecting a buffer */
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  /* Puts data in the buffer */
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
  /* Enable the attributes */
  glEnableVertexAttribArray(0);
  /* Defines the vertex attributes */
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  ShaderProgramSource source = ParseShader("res/shaders/triangle.shader");

  unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
  glUseProgram(shader);

  /* Loop until the user closes the window */
  while(!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Modern OpenGL drawing */
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteProgram(shader);

  glfwTerminate();
  return EXIT_SUCCESS;
}