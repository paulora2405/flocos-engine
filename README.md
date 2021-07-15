# Flocos Engine
*Paulo Roberto Albuquerque*

---

## About

Flocos Engine is a simple OpenGL abstraction library, not exactly fully fledged a Renderer or Graphics Engine, but a work in progress.
There is no final objective and no strict goals, it simply exists for learning and entertainment purposes.

### Building and Compiling

To build the project, do:
1. `$ mkdir build && cd build`
1. `$ cmake ..`
1. `$ make`

### Executing

The library is a shared library, meaning it can be attached to other existing project, not necessarily the existing entry point.
To execute the included SandBox app, do:
`$ ./build/Sandbox/src/FlocosSandbox.out`

### Dependencies

In order to build, compile and execute the project you need:
- `CMake >= 3.3`
- `C++17 compiler`
- `GLEW`
- `glfw`
