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

    // iluminacion
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

    // piso
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-20, 0, -20);
    glVertex3f(-20, 0, 20);
    glVertex3f(20, 0, 20);
    glVertex3f(20, 0, -20);
    glEnd();

    robot.draw();
    glutSwapBuffers();
}

void update(int value) {
    float moveSpeed = 0.2f;
    float turnSpeed = 3.0f;
    
    if (robot.moveState == MOVING_FORWARD) robot.moveForward(moveSpeed);
    else if (robot.moveState == MOVING_BACKWARD) robot.moveForward(-moveSpeed * 0.5f);

    if (keys['a'] || keys['A']) robot.turn(turnSpeed);
    if (keys['d'] || keys['D']) robot.turn(-turnSpeed);

    robot.update(0.016f);
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key < 256) keys[key] = true;
    
    switch (key) {
    case 'w': case 'W': robot.toggleForward(); break;
    case 's': case 'S': robot.toggleBackward(); break;
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
            lastMouseX = x;
            lastMouseY = y;

            // modo avion: arrastrar helice con el mouse
            if (robot.targetForm == PLANE && robot.currentForm == PLANE &&
                robot.transformFactor >= 1.0f && robot.hitTestRobotBody(x, y)) {
                robot.isDraggingPropeller = true;
                isDragging = false;
            } else if (robot.isEditMode) {
                int cp = robot.hitTestControlPoint(x, y);
                if (cp >= 0) {
                    robot.startDragJoint(cp);
                    isDragging = false;
                } else {
                    robot.toggleEditMode();
                    isDragging = true;
                }
            } else {
                if (robot.canEdit() && robot.hitTestRobotBody(x, y)) {
                    robot.toggleEditMode();
                    isDragging = false;
                } else {
                    isDragging = true;
                }
            }
        } else {
            robot.endDragJoint();
            robot.isDraggingPropeller = false;
            isDragging = false;
        }
    } else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            isZooming = true;
            lastMouseY = y;
        } else {
            isZooming = false;
        }
    } else if (button == 3) {
        if (state == GLUT_DOWN) camera.zoom(-1.5f);
    } else if (button == 4) {
        if (state == GLUT_DOWN) camera.zoom(1.5f);
    }
}

void motion(int x, int y) {
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    if (robot.isDraggingPropeller) {
        robot.propellerAngle += (float)dx * 2.0f;
        if (robot.propellerAngle > 360.0f) robot.propellerAngle -= 360.0f;
        if (robot.propellerAngle < 0.0f) robot.propellerAngle += 360.0f;
    } else if (robot.isDraggingJoint) {
        robot.dragJoint((float)dx);
    } else if (isDragging) {
        camera.rotate((float)dx * 0.5f, (float)dy * 0.5f);
    } else if (isZooming) {
        camera.zoom((float)dy * 0.1f);
    }

    lastMouseX = x;
    lastMouseY = y;
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
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16, update, 0);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glutMainLoop();
    return 0;
}
