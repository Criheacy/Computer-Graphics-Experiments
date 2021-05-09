#pragma once

#include <cmath>
#include <queue>
#include <vector>
#include <random>
#include <string>
#include <memory>
#include <algorithm>

#include <glut.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

#define PI acosf(-1)
#define DRAW_GIZMOS true

#define BACKGROUND_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define LINE_COLOR glm::vec3(0.0f, 0.0f, 0.0f)

#define POLYGON_VERTEX_COLOR glm::vec3(0.2f, 0.2f, 0.2f)
#define POLYGON_EDGE_COLOR glm::vec3(0.2f, 0.2f, 0.2f)
#define POLYGON_HOVER_COLOR glm::vec3(1.0f, 0.65f, 0.0f)
#define POLYGON_PRESS_COLOR glm::vec3(0.0f, 0.0f, 0.0f)

#define CURVE_COLOR glm::vec3(1.0f, 0.65f, 0.0f)
#define CURVE_LINE_WIDTH 1
#define BEZIER_CURVE_PRECISION 50

#define ENABLE_HOVERING_HINT true

#define LINE_WIDTH 1
#define HOVERING_LINE_WIDTH 2
#define HOVERING_VERTEX_OUTLINE_RADIUS 1
#define HOVERING_VERTEX_RADIUS 0.75

#define FONT_TYPE GLUT_BITMAP_9_BY_15
#define FONT_WIDTH 9
#define FONT_HEIGHT 15

#define GIZMO_SCALE 8
#define VERTEX_MAXSIZE 500