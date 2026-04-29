#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Robot.h"
#include "Camera.h"
#include <iostream>

Robot robot;
Camera camera;
int lastMouseX, lastMouseY;
bool isDragging = false;
bool isZooming = false;

void init() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    // Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    GLfloat lightPos[] = { 5.0f, 10.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    camera.apply();
    
    // Draw Ground
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-20, 0, -20);
    glVertex3f(-20, 0, 20);
    glVertex3f(20, 0, 20);
    glVertex3f(20, 0, -20);
    glEnd();

    // Draw Robot
    robot.draw();
    
    glutSwapBuffers();
}

void update(int value) {
    robot.update(0.016f); // Approx 60fps
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.2f;
    float turnSpeed = 5.0f;
    switch (key) {
    case 'w': case 'W': robot.moveForward(moveSpeed); break;
    case 's': case 'S': robot.moveForward(-moveSpeed); break;
    case 'a': case 'A': robot.turn(turnSpeed); break;
    case 'd': case 'D': robot.turn(-turnSpeed); break;
    case '1': robot.setForm(HUMANOID); break;
    case '2': robot.setForm(CAR); break;
    case '3': robot.setForm(PLANE); break;
    case '4': robot.setForm(BOAT); break;
    case 'g': case 'G': robot.greet(); break;
    case 27: exit(0); break;
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            isDragging = false;
        }
    } else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            isZooming = true;
            lastMouseY = y;
        } else {
            isZooming = false;
        }
    }
}

void motion(int x, int y) {
    if (isDragging) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        camera.rotate((float)dx * 0.5f, (float)dy * 0.5f);
        lastMouseX = x;
        lastMouseY = y;
    } else if (isZooming) {
        int dy = y - lastMouseY;
        camera.zoom((float)dy * 0.1f);
        lastMouseY = y;
    }
    glutPostRedisplay();
}

void mouseWheel(int wheel, int direction, int x, int y) {
    camera.zoom(direction > 0 ? -1.0f : 1.0f);
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 768);
    glutCreateWindow("Transformers Robot Project");
    
    glewInit();
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMouseWheelFunc(mouseWheel);
    glutTimerFunc(16, update, 0);
    
    std::cout << "Controls:" << std::endl;
    std::cout << "WASD: Move Robot" << std::endl;
    std::cout << "1: Humanoid, 2: Car, 3: Plane, 4: Boat" << std::endl;
    std::cout << "Left Click + Drag: Rotate Camera" << std::endl;
    std::cout << "Right Click + Drag / Mouse Wheel: Zoom" << std::endl;
    std::cout << "ESC: Exit" << std::endl;

    glutMainLoop();
    return 0;
}
