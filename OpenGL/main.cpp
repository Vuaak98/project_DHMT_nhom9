#include "Angel.h"
#include "cube.h"
#include "sphere.h"

GLuint program;
GLfloat theta[3] = { 0, 0, 0 };
GLfloat dr = 5;
mat4 model;
GLuint model_loc;
mat4 projection;
GLuint projection_loc;
mat4 view;
GLuint view_loc;

void display(void)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 rotation = RotateX(theta[0]) * RotateY(theta[1]) * RotateZ(theta[2]);

    // HLP bên trái
    mat4 cubeModel = Translate(-1.0, 0.0, 0.0) * Scale(1.0, 1.0, 1.0) * rotation;
    drawCubeWithLighting(program, model_loc, cubeModel);
    // Hình cầu bên phải
    mat4 sphereModel = Translate(1.0, 0.0, 0.0) * Scale(1.0, 1.0, 1.0) * rotation;
    drawSphereWithLighting(program, model_loc, sphereModel);





    glutSwapBuffers();
}


void reshape(int width, int height)
{
    vec4 eye(0, 0, 5, 1);
    vec4 at(0, 0, 0, 1);
    vec4 up(0, 1, 0, 1);
    view = LookAt(eye, at, up);
    glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
    projection = Frustum(-4, 4, -4, 4, 2, 10); // mở rộng vùng nhìn
    glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
    glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 033: // Escape key
        exit(1);
        break;
    case 'x':
        theta[0] += dr; // Cập nhật góc xoay theo trục X
        glutPostRedisplay();
        break;
    case 'X':
        theta[0] -= dr;
        glutPostRedisplay();
        break;
    case 'y':
        theta[1] += dr; // Cập nhật góc xoay theo trục Y
        glutPostRedisplay();
        break;
    case 'Y':
        theta[1] -= dr;
        glutPostRedisplay();
        break;
    case 'z':
        theta[2] += dr; // Cập nhật góc xoay theo trục Z
        glutPostRedisplay();
        break;
    case 'Z':
        theta[2] -= dr;
        glutPostRedisplay();
        break;
    }
}

void shaderSetup(void)
{
    program = InitShader("vshader1.glsl", "fshader1.glsl");
    glUseProgram(program);
    model_loc = glGetUniformLocation(program, "Model");
    projection_loc = glGetUniformLocation(program, "Projection");
    view_loc = glGetUniformLocation(program, "View");
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("A Cube and Sphere with Lighting");
    glewInit();

    // Khởi tạo hình lập phương
    generateGeometry();
    initGPUBuffers();
    // Khởi tạo hình cầu
    initSphereGeometry();




    // Thiết lập shader
    shaderSetup();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
