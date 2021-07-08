#include "graphics/graphics_engine.hpp"

#include "graphics/gui.hpp"
#include "graphics/index_buffer.hpp"
#include "graphics/renderer.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/vertex_array.hpp"
#include "graphics/vertex_buffer.hpp"
#include "graphics/vertex_buffer_layout.hpp"
#include "inputs/inputs.hpp"
#include "tests/test_batch_render.hpp"
#include "tests/test_clear_color.hpp"
#include "tests/test_grid.hpp"
#include "tests/test_texture.hpp"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"

namespace GE {

void GraphicsEngine::mainLoop() {
  Renderer re;

  Gui gui{this->m_window, false};

  tests::Test* currentTest = nullptr;
  tests::TestMenu* testMenu = new tests::TestMenu{currentTest};
  currentTest = testMenu;

  testMenu->registerTest<tests::TestClearColor>("TestClearColor");
  testMenu->registerTest<tests::TestTexture2D>("TestTexture2D");
  testMenu->registerTest<tests::TestGrid>("TestGrid");
  testMenu->registerTest<tests::TestBatchRender>("TestBatchRender");

  while(!glfwWindowShouldClose(this->m_window)) {
    re.clear();

    gui.newFrame();

    if(currentTest) {
      currentTest->onUpdate(0.0f);
      currentTest->onRender();
      ImGui::Begin("TestMenu");
      if(currentTest != testMenu && ImGui::Button("<-")) {
        delete currentTest;
        currentTest = testMenu;
      }
      currentTest->onImGuiRender();
      ImGui::End();
    }

    gui.draw();

    this->swapAndPoll();
  }

  if(currentTest != testMenu) delete testMenu;
  delete currentTest;
}

void GraphicsEngine::swapAndPoll() {
  glfwSwapBuffers(this->m_window);
  glfwPollEvents();
}

GLFWwindow* GraphicsEngine::getWindow() const {
  return m_window;
}

unsigned short GraphicsEngine::getWindowWidth() const {
  return m_width;
}
unsigned short GraphicsEngine::getWindowHeight() const {
  return m_height;
}

void GraphicsEngine::init() {
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
  std::string window_name{std::to_string(m_width) + "x" + std::to_string(m_height)};
  m_window = glfwCreateWindow(m_width, m_height, window_name.c_str(), NULL, NULL);
  if(!m_window) {
    LOG(ERROR) << "GLFW COULD NOT CREATE THE WINDOW";
    glfwTerminate();
    return;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(m_window);

  /* Window's properties */
  // glfwSetWindowAspectRatio(m_window, m_width, m_height);
  // glfwSetWindowPos(m_window, 200, 150);
  // GLFWmonitor* monitor = glfwGetWindowMonitor(m_window);
  // const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  // glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height,
  //                      mode->refreshRate);

  /* Sincronizes the framerate with the monitors refresh rate */
  glfwSwapInterval(1);

  /* Initialize GLEW library */
  if(glewInit() != GLEW_OK) {
    LOG(ERROR) << "GLEW COULD NOT INITIALIZE";
    return;
  }

  /* Enable blending in order to transparency to work */
  GLCALL(glEnable(GL_BLEND));
  GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  /* Print OpenGL Version */
  LOG(INFO) << "OpenGL Version: " << glGetString(GL_VERSION);
  LOG(INFO) << "Renderer: " << glGetString(GL_RENDERER);
  LOG(INFO) << "Vendor: " << glGetString(GL_VENDOR);

  /* Window initiated successfully */
  LOG(INFO) << "Window initiated successfully";

  /* Callbacks setting */
  glfwSetKeyCallback(this->m_window, inputs::handleKeyboard);
}

GraphicsEngine& GraphicsEngine::getInstance(const unsigned short& width,
                                            const unsigned short& height) {
  static GraphicsEngine s_Instance{width, height};
  return s_Instance;
}

GraphicsEngine::GraphicsEngine(const unsigned short& width, const unsigned short& height)
    : m_width{width},
      m_height{height} {
  this->init();
}

GraphicsEngine::~GraphicsEngine() {
  glfwTerminate();
  LOG(INFO) << "Window terminated successfully";
}

}  // namespace GE