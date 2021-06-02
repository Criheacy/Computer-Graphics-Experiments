cmake_minimum_required(VERSION 3.19)
project(7-HalfEdgeStructure)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(OpenGL main.cpp glad.c shader.cpp shader.h graphics.cpp graphics.h space.cpp space.h graphics/cube.cpp graphics/cube.h graphics/tetrahedron.cpp graphics/tetrahedron.h graphics/sphere.cpp graphics/sphere.h graphics/offgraphic.cpp graphics/offgraphic.h)
target_link_libraries(OpenGL ${OPENGL_LIBRARY} glfw3 GL m Xrandr Xi X11 Xxf86vm pthread dl Xinerama Xcursor)
