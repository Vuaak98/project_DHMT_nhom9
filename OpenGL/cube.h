#ifndef CUBE_H
#define CUBE_H

#include "Angel.h"

// Định nghĩa lại các kiểu dữ liệu nếu cần thiết (hoặc sử dụng trực tiếp vec4 từ Angel.h)
typedef vec4 point4;
typedef vec4 color4;

extern const int NumPoints;
extern point4 points[];
extern color4 colors[];
extern vec3 normals[];
extern point4 vertices[];
extern color4 vertex_colors[];
extern GLuint vao_cube, vbo_cube; 
void generateGeometry();
void initGPUBuffers();
void drawCubeWithLighting(GLuint program, GLuint model_loc, mat4 model);

#endif
