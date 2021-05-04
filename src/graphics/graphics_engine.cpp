#include "graphics/graphics_engine.hpp"

namespace GE {

void GraphicsEngine::geMainLoop() {
  float positions[] = {
      -0.5f, -0.5f, 0.0f, 0.0f,  // 0
      0.5f,  -0.5f, 1.0f, 0.0f,  // 1
      0.5f,  0.5f,  1.0f, 1.0f,  // 2
      -0.5f, 0.5f,  0.0f, 1.0f   // 3
  };
  unsigned int indices[] = {
      0, 1, 2,  // first triangle
      2, 3, 0,  // second triangle
  };

  VertexArray va;
  VertexBuffer vb{positions, 4 * 4 * sizeof(float)};

  VertexBufferLayout layout;
  layout.push<float>(2);
  layout.push<float>(2);
  va.addBuffer(vb, layout);

  IndexBuffer ib{indices, 6};

  Shader shader{"res/shaders/triangle_vf.shader"};
  shader.bind();
  // shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

  Texture tex{"res/textures/c++.png"};
  tex.bind();
  shader.setUniform1i("u_Texture", 0);

  va.unbind();
  vb.unbind();
  ib.unbind();
  shader.unbind();

  // float r = 0.0f, g = 0.8f, b = 0.8f;

  Renderer re;

  while(!glfwWindowShouldClose(this->m_window)) {
    re.clear();

    shader.bind();
    // shader.setUniform4f("u_Color", r, g, b, 1.0f);
    // r = r + 0.1f > 1.0f ? 0.0f : r + 0.05f;
    // g = g + 0.1f > 1.0f ? 0.0f : g + 0.05f;
    // b = b + 0.1f > 1.0f ? 0.0f : b + 0.05f;

    re.draw(va, ib, shader);

    glfwSwapBuffers(this->m_window);
    glfwPollEvents();
  }
}

GLFWwindow* GraphicsEngine::geGetWindow() const {
  return m_window;
}

void GraphicsEngine::geInit() {
  LOG(INFO) << "Initializing the GraphicsEngine instance";
  /* Initialize GLFW library */
  if(!glfwInit()) {
    LOG(ERROR) << "GLFW COULD NOT INITIALIZE";
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  // not working
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);  // not working
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  std::string window_name{std::to_string(m_width) + "x" +
                          std::to_string(m_height)};
  m_window =
      glfwCreateWindow(m_width, m_height, window_name.c_str(), NULL, NULL);
  if(!m_window) {
    glfwTerminate();
    return;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_window);

  /* Sincronizes the framerate with the monitors refresh rate */
  glfwSwapInterval(1);

  /* Initialize GLEW library */
  if(glewInit() != GLEW_OK) {
    LOG(ERROR) << "ERROR: GLEW COULD NOT INITIALIZE";
    return;
  }

  GLCALL(glEnable(GL_BLEND));
  GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  /* Print OpenGL Version */
  LOG(INFO) << "OpenGL Version: " << glGetString(GL_VERSION);

  /* Window initiated successfully */
  LOG(INFO) << "Window initiated successfully";
}

GraphicsEngine::GraphicsEngine(const unsigned short& width,
                               const unsigned short& height)
    : m_width{width},
      m_height{height} {
  this->geInit();
}

GraphicsEngine::~GraphicsEngine() {
  glfwTerminate();
  LOG(INFO) << "Window terminated successfully";
}

}  // namespace GE