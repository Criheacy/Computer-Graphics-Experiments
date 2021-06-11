cmake_minimum_required(VERSION 3.19)
project(7-HalfEdgeStructure)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(7-HalfEdgeStructure main.cpp glad.c shader.cpp shader.h graphics/graphics.cpp graphics/graphics.h space.cpp space.h graphics/cube.cpp graphics/cube.h graphics/tetrahedron.cpp graphics/tetrahedron.h graphics/sphere.cpp graphics/sphere.h graphics/offgraphic.cpp graphics/offgraphic.h graphics/graphics_component.cpp graphics/graphics_component.h utils/input_handler.cpp utils/input_handler.h utils/glfw_toolkit.cpp utils/glfw_toolkit.h utils/progress_bar.cpp utils/progress_bar.h)
target_link_libraries(7-HalfEdgeStructure ${OPENGL_LIBRARY} glfw3 GL m Xrandr Xi X11 Xxf86vm pthread dl Xinerama Xcursor)
