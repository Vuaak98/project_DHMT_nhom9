#include "cube.h"

const int NumPoints = 36;
point4 points[NumPoints];
color4 colors[NumPoints];
vec3 normals[NumPoints];
point4 vertices[8];
color4 vertex_colors[8];

void initCube()
{
    // Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
    vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
    vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
    vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
    vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
    vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
    vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
    vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
    vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

    // Gán giá trị màu sắc cho các đỉnh của hình lập phương
    vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
    vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
    vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
    vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
    vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
    vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
    vertex_colors[6] = color4(1.0, 0.5, 0.0, 1.0); // orange
    vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
    printf("vao_cube = %u, vbo_cube = %u\\n", vao_cube, vbo_cube);
}

int Index = 0;
void quad(int a, int b, int c, int d)
{
    vec4 u = vertices[b] - vertices[a];
    vec4 v = vertices[c] - vertices[b];
    vec3 normal = normalize(cross(u, v));

    normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
    normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
    normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
    normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
}

void makeColorCube()
{
    quad(1, 0, 3, 2);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(6, 5, 1, 2);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);
}

void generateGeometry()
{
    initCube();
    makeColorCube();
}
GLuint vao_cube;
GLuint vbo_cube;
void initGPUBuffers()
{
    glGenVertexArrays(1, &vao_cube);
    glBindVertexArray(vao_cube);

    glGenBuffers(1, &vbo_cube);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);

    // Setup thuộc tính tại đây
    GLuint loc_vPosition = 0; // layout location 0
    glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(loc_vPosition);
    GLuint loc_vColor = 1; // layout location 1
    glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
    glEnableVertexAttribArray(loc_vColor);
    GLuint loc_vNormal = 2; // layout location 2
    glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));
    glEnableVertexAttribArray(loc_vNormal);
    // Unbind VAO
    glBindVertexArray(0);
}

void drawCubeWithLighting(GLuint program, GLuint model_loc, mat4 model)
{
    glBindVertexArray(vao_cube);
    glUseProgram(program);

    // Thiết lập tham số chiếu sáng
    point4 light_position(0.0, 0.0, 1.0, 0.0);
    color4 light_ambient(0.2, 0.2, 0.2, 1.0);
    color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
    color4 light_specular(1.0, 1.0, 1.0, 1.0);
    color4 material_ambient(1.0, 0.0, 1.0, 1.0);
    color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
    color4 material_specular(1.0, 0.8, 0.0, 1.0);
    float material_shininess = 100.0;

    color4 ambient_product = light_ambient * material_ambient;
    color4 diffuse_product = light_diffuse * material_diffuse;
    color4 specular_product = light_specular * material_specular;

    glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
    glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
    glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
    glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
    glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

    // Vẽ hình lập phương
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);
    glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}