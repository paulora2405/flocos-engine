#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <iostream>
#include <string>

short WIDTH;
short HEIGHT;

int main(int argc, char const* argv[]) {
  if(argc == 1)
    WIDTH = 800, HEIGHT = 640;
  else if(argc == 3)
    WIDTH = atoi(argv[1]), HEIGHT = atoi(argv[2]);
  else {
    std::cerr << "ARGUMENT ERROR" << std::endl;
    return EXIT_FAILURE;
  }

  GLFWwindow* window;

  /* Initialize GLFW library */
  if(!glfwInit()) return -1;

  /* Create a windowed mode window and its OpenGL context */
  std::string window_name{std::to_string(WIDTH) + "," + std::to_string(HEIGHT)};
  window = glfwCreateWindow(WIDTH, HEIGHT, window_name.c_str(), NULL, NULL);
  if(!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Initialize GLEW library */
  if(glewInit() != GLEW_OK) {
    std::cerr << "GLEW COULD NOT INITIALIZE" << std::endl;
    return EXIT_FAILURE;
  }

  /* Print OpenGL Version */
  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

  /* Loop until the user closes the window */
  while(!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Test legacy OpenGL */
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}