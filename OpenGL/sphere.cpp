#include "sphere.h"
#include <vector>

const int SphereStacks = 10;
const int SphereSlices = 10;
const int SpherePoints = SphereStacks * SphereSlices * 6;
point4 spherePoints[SpherePoints];
color4 sphereColors[SpherePoints];
vec3 sphereNormals[SpherePoints];

GLuint vao_sphere;
GLuint vbo_sphere;

void initSphereGeometry()
{
    // Sinh dữ liệu cho sphere
    std::vector<point4> vertices;
    float radius = 0.35f;
    for (int stack = 0; stack <= SphereStacks; ++stack) {
        float phi = stack * M_PI / SphereStacks;
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);
        for (int slice = 0; slice <= SphereSlices; ++slice) {
            float theta = slice * 2 * M_PI / SphereSlices;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);
            float x = cosTheta * sinPhi;
            float y = cosPhi;
            float z = sinTheta * sinPhi;
            vertices.push_back(point4(x * radius, y * radius, z * radius, 1.0));
        }
    }
    int index = 0;
    int numSlices = SphereSlices + 1;
    for (int stack = 0; stack < SphereStacks; ++stack) {
        for (int slice = 0; slice < SphereSlices; ++slice) {
            int currRow = stack * numSlices;
            int nextRow = (stack + 1) * numSlices;
            // Triangle 1
            spherePoints[index] = vertices[currRow + slice];
            sphereNormals[index] = vec3(spherePoints[index].x, spherePoints[index].y, spherePoints[index].z);
            sphereColors[index] = color4(0.0, 1.0, 1.0, 1.0);
            index++;
            spherePoints[index] = vertices[nextRow + slice];
            sphereNormals[index] = vec3(spherePoints[index].x, spherePoints[index].y, spherePoints[index].z);
            sphereColors[index] = color4(0.0, 1.0, 1.0, 1.0);
            index++;
            spherePoints[index] = vertices[currRow + slice + 1];
            sphereNormals[index] = vec3(spherePoints[index].x, spherePoints[index].y, spherePoints[index].z);
            sphereColors[index] = color4(0.0, 1.0, 1.0, 1.0);
            index++;
            // Triangle 2
            spherePoints[index] = vertices[nextRow + slice];
            sphereNormals[index] = vec3(spherePoints[index].x, spherePoints[index].y, spherePoints[index].z);
            sphereColors[index] = color4(0.0, 1.0, 1.0, 1.0);
            index++;
            spherePoints[index] = vertices[nextRow + slice + 1];
            sphereNormals[index] = vec3(spherePoints[index].x, spherePoints[index].y, spherePoints[index].z);
            sphereColors[index] = color4(0.0, 1.0, 1.0, 1.0);
            index++;
            spherePoints[index] = vertices[currRow + slice + 1];
            sphereNormals[index] = vec3(spherePoints[index].x, spherePoints[index].y, spherePoints[index].z);
            sphereColors[index] = color4(0.0, 1.0, 1.0, 1.0);
            index++;
        }
    }
    // Tạo VAO và VBO cho hình cầu
    glGenVertexArrays(1, &vao_sphere);
    glBindVertexArray(vao_sphere);
    glGenBuffers(1, &vbo_sphere);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sphere);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spherePoints) + sizeof(sphereColors) + sizeof(sphereNormals), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(spherePoints), spherePoints);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(spherePoints), sizeof(sphereColors), sphereColors);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(spherePoints) + sizeof(sphereColors), sizeof(sphereNormals), sphereNormals);

    // Setup thuộc tính tại đây
    GLuint loc_vPosition = 0; // layout location 0
    glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(loc_vPosition);
    GLuint loc_vColor = 1; // layout location 1
    glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(spherePoints)));
    glEnableVertexAttribArray(loc_vColor);
    GLuint loc_vNormal = 2; // layout location 2
    glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(spherePoints) + sizeof(sphereColors)));
    glEnableVertexAttribArray(loc_vNormal);
    // Unbind VAO
    glBindVertexArray(0);
    printf("vao_sphere = %u, vbo_sphere = %u\\n", vao_sphere, vbo_sphere);
}
void drawSphereWithLighting(GLuint program, GLuint model_loc, mat4 model)
{
    glBindVertexArray(vao_sphere);
    glUseProgram(program);

    // Thiết lập tham số chiếu sáng
    point4 light_position(0.0, 0.0, 1.0, 0.0);
    color4 light_ambient(0.2, 0.2, 0.2, 1.0);
    color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
    color4 light_specular(1.0, 1.0, 1.0, 1.0);
    color4 material_ambient(0.0, 1.0, 1.0, 1.0); // Cyan
    color4 material_diffuse(0.0, 0.8, 0.8, 1.0);
    color4 material_specular(0.0, 0.8, 0.8, 1.0);
    float material_shininess = 80.0;

    color4 ambient_product = light_ambient * material_ambient;
    color4 diffuse_product = light_diffuse * material_diffuse;
    color4 specular_product = light_specular * material_specular;

    glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
    glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
    glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
    glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
    glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

    // Vẽ hình cầu
    glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);
    glDrawArrays(GL_TRIANGLES, 0, SpherePoints);
}
