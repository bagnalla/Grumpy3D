#ifndef BAGNALL_H
#define BAGNALL_H

#define _CRT_SECURE_NO_DEPRECATE
#define GLUT_DISABLE_ATEXIT_HACK
#define GLM_FORCE_RADIANS

#define BUFFER_OFFSET(offset)   ((GLvoid*) (offset))

//#include <glm/glm.hpp>
//#include <glm/vec2.hpp>
//#include <glm/vec3.hpp>
//#include <glm/vec4.hpp>
//#include <glm/mat4x4.hpp>
//#include <glm/matrix.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/transform.hpp>
//#include <glm/gtx/fast_trigonometry.hpp> // for wrapAngle
//#include <glm/gtc/type_ptr.hpp>
#include "GlmHeaders.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>

const GLfloat DegreesToRadians = M_PI / 180.0;

using namespace glm;

#endif
