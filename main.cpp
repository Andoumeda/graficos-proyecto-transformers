#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Robot.h"
#include "Camera.h"
#include <iostream>
#include <cstdlib>

Robot robot;
Camera camera;
int lastMouseX, lastMouseY;
bool isDragging = false;
bool isZooming = false;

bool keys[256];

void init() {
    for (int i = 0; i < 256; i++) keys[i] = false;

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
    float moveSpeed = 0.2f;
    float turnSpeed = 3.0f;
    
    if (keys['w'] || keys['W']) robot.moveForward(moveSpeed);
    if (keys['s'] || keys['S']) robot.moveForward(-moveSpeed);
    if (keys['a'] || keys['A']) robot.turn(turnSpeed);
    if (keys['d'] || keys['D']) robot.turn(-turnSpeed);

    robot.update(0.016f);
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key < 256) keys[key] = true;
    
    switch (key) {
    case '1': robot.setForm(HUMANOID); break;
    case '2': robot.setForm(CAR); break;
    case '3': robot.setForm(BOAT); break;
    case '4': robot.setForm(PLANE); break;
    case 'g': case 'G': robot.greet(); break;
    case 't': case 'T': robot.shoot(); break;
    case 27: exit(0); break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    if (key < 256) keys[key] = false;
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
    } else if (button == 3) { // Rueda arriba
        if (state == GLUT_DOWN) camera.zoom(-1.5f);
    } else if (button == 4) { // Rueda abajo
        if (state == GLUT_DOWN) camera.zoom(1.5f);
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
    glutInitWindowSize(800, 600);
    glutCreateWindow("Transformer Robot");

    glewInit();
    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp); // ADDED THIS
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16, update, 0);

    // Setup basic projection
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glutMainLoop();
    return 0;
}
