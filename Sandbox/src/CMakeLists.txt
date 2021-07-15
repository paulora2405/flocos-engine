include_directories(${CMAKE_SOURCE_DIR}/Sandbox/include)
include_directories(${CMAKE_SOURCE_DIR}/FlocosEngine/include)

link_directories(${CMAKE_SOURCE_DIR}/FlocosEngine/src)

add_executable(${PROJECT_NAME}.out
  ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
)

target_link_libraries(${PROJECT_NAME}.out
  FlocosLibrary
  m
  GL
  GLU
  GLEW
  glfw
)