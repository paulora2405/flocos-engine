include_directories(${CMAKE_SOURCE_DIR}/FlocosEngine/include)
include_directories(${CMAKE_SOURCE_DIR}/FlocosEngine/include/vendor/imgui)

add_library(
  FlocosLibrary SHARED

  graphics/graphics_engine.cpp
  graphics/gui.cpp
  graphics/index_buffer.cpp
  graphics/renderer.cpp
  graphics/shader.cpp
  graphics/texture.cpp
  graphics/vertex_array.cpp
  graphics/vertex_buffer_layout.cpp
  graphics/vertex_buffer.cpp

  inputs/inputs.cpp

  logging/gl_error.cpp

  tests/test_batch_render.cpp
  tests/test_clear_color.cpp
  tests/test_grid.cpp
  tests/test_texture.cpp
  tests/test.cpp

  vendor/easylogging/easylogging++.cpp

  vendor/imgui/imgui_demo.cpp
  vendor/imgui/imgui_draw.cpp
  vendor/imgui/imgui_example.cpp
  vendor/imgui/imgui_impl_glfw.cpp
  vendor/imgui/imgui_impl_opengl3.cpp
  vendor/imgui/imgui_tables.cpp
  vendor/imgui/imgui_widgets.cpp
  vendor/imgui/imgui.cpp

  vendor/stbi/stb_image.cpp
)