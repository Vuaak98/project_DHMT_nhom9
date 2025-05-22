#ifndef SPHERE_H
#define SPHERE_H

#include "Angel.h"

// Định nghĩa lại các kiểu dữ liệu nếu cần
typedef vec4 point4;
typedef vec4 color4;
extern GLuint vao_sphere, vbo_sphere; 
extern const int SpherePoints;
void initSphereGeometry();
void drawSphereWithLighting(GLuint program, GLuint model_loc, mat4 model);

#endif
