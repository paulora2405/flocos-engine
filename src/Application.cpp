#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include <iostream>

short width;
short height;

int main(int argc, char const *argv[]) {
  if(argc == 1)
    width = 800, height = 640;
  else if(argc == 3)
    width = atoi(argv[1]), height = atoi(argv[2]);
  else {
    std::cerr << "ARGUMENT ERROR" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << width << ',' << height << std::endl;
}