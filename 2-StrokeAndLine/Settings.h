#pragma once

#include <cmath>
#include <vector>
#include <string>

#include <GL/glut.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700
#define NEG_INT_MIN -2147483648

#define PI acosf(-1)
#define DRAW_GIZMOS true

#define BACKGROUND_COLOR glm::vec3(1.0f, 1.0f, 1.0f)
#define LINE_COLOR glm::vec3(0.0f, 0.0f, 0.0f)
#define MARK_COLOR glm::vec3(0.5f, 0.5f, 0.5f)
#define HOVER_COLOR glm::vec3(0.92f, 0.92f, 0.92f)
#define GRID_COLOR glm::vec3(0.9f, 0.9f, 0.9f)

#define STROKE_WIDTH_RATIO 0.05f
#define STROKE_WIDTH_MAX 5

#define SHAPE_NONE 0
#define SHAPE_LINE 1
#define SHAPE_CIRCLE 2

#define ENABLE_SCALE_CONSTRAINT true
#define SCALE_CONSTRAINT_MAX 100
#define SCALE_CONSTRAINT_MIN 1

#define ENABLE_SCALE_AT_POINT true

#define ENABLE_HOVERING_HINT true

#define FADE_FRAME_LINES_WHEN_SCALE 7

#define FONT_TYPE GLUT_BITMAP_9_BY_15
#define FONT_WIDTH 9
#define FONT_HEIGHT 15