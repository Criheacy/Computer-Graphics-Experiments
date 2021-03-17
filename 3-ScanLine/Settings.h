#pragma once

#include <cmath>
#include <vector>
#include <random>
#include <string>
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
#define MARK_COLOR glm::vec3(0.95f, 0.2f, 0.0f)
#define GRID_COLOR glm::vec3(0.9f, 0.9f, 0.9f)

#define SCANNING_LINE_COLOR glm::vec3(0.12f, 0.27f, 0.95f)
#define POINT_ON_SCANNING_LINE_COLOR glm::vec3(0.0f, 0.0f, 0.0f)
#define SCANNING_LINE_BLINKING_COLOR glm::vec3(1.0f, 1.0f, 1.0f)

#define POLYGON_VERTEX_COLOR glm::vec3(0.2f, 0.2f, 0.2f)
#define POLYGON_EDGE_COLOR glm::vec3(0.2f, 0.2f, 0.2f)
#define POLYGON_HOVER_COLOR glm::vec3(0.2f, 0.2f, 0.2f)
#define POLYGON_PRESS_COLOR glm::vec3(0.0f, 0.0f, 0.0f)

#define STROKE_WIDTH_MAX 3

#define ENABLE_SCALE_CONSTRAINT true
#define SCALE_CONSTRAINT_MAX 50
#define SCALE_CONSTRAINT_MIN 1

#define ENABLE_SCALE_AT_POINT true

#define ENABLE_HOVERING_HINT true

#define FADE_FRAME_LINES_WHEN_SCALE 7

#define FONT_TYPE GLUT_BITMAP_9_BY_15
#define FONT_WIDTH 9
#define FONT_HEIGHT 15

#define GIZMO_SCALE 8