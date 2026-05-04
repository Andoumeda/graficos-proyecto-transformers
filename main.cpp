#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Robot.h"
#include "Camera.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>

Robot robot;
Camera camera;
int lastMouseX, lastMouseY;
bool isDragging = false;
bool isZooming = false;
int windowWidth = 800;
int windowHeight = 600;

bool keys[256];


const float PI_F = 3.1415926535f;

struct Vec3 {
    float x, y, z;
};

Vec3 sunPosition = { -7.0f, 12.0f, 6.0f };

void drawEllipseXZ(float radiusX, float radiusZ, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float a = (2.0f * PI_F * i) / segments;
        glVertex3f(cosf(a) * radiusX, 0.0f, sinf(a) * radiusZ);
    }
    glEnd();
}

void drawSkyBackground() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);

    // Gradiente de cielo. Es 2D para funcionar bien con la tuberia fija de OpenGL.
    glBegin(GL_QUADS);
    glColor3f(0.42f, 0.72f, 1.00f); // horizonte
    glVertex2f(0.0f, 0.0f);
    glVertex2f((float)windowWidth, 0.0f);
    glColor3f(0.05f, 0.22f, 0.55f); // parte alta
    glVertex2f((float)windowWidth, (float)windowHeight);
    glVertex2f(0.0f, (float)windowHeight);
    glEnd();

    // Nubes simples hechas con circulos 2D translúcidos.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.55f);

    auto cloudCircle = [](float cx, float cy, float rx, float ry) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= 32; ++i) {
            float a = (2.0f * PI_F * i) / 32.0f;
            glVertex2f(cx + cosf(a) * rx, cy + sinf(a) * ry);
        }
        glEnd();
    };

    cloudCircle(windowWidth * 0.22f, windowHeight * 0.78f, 46.0f, 15.0f);
    cloudCircle(windowWidth * 0.27f, windowHeight * 0.80f, 58.0f, 20.0f);
    cloudCircle(windowWidth * 0.34f, windowHeight * 0.77f, 42.0f, 14.0f);

    cloudCircle(windowWidth * 0.68f, windowHeight * 0.72f, 42.0f, 14.0f);
    cloudCircle(windowWidth * 0.73f, windowHeight * 0.75f, 60.0f, 20.0f);
    cloudCircle(windowWidth * 0.80f, windowHeight * 0.72f, 44.0f, 14.0f);

    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void setupWorldLighting() {
    // La posicion de la luz se carga despues de camera.apply(), por eso queda en coordenadas del mundo.
    GLfloat lightPos[] = { sunPosition.x, sunPosition.y, sunPosition.z, 1.0f };
    GLfloat diffuse[]  = { 1.00f, 0.92f, 0.78f, 1.0f };
    GLfloat specular[] = { 0.75f, 0.70f, 0.62f, 1.0f };
    GLfloat ambient[]  = { 0.22f, 0.25f, 0.30f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
}

void drawSun() {
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glColor3f(1.0f, 0.86f, 0.18f);
    glPushMatrix();
    glTranslatef(sunPosition.x, sunPosition.y, sunPosition.z);
    glutSolidSphere(0.35f, 24, 24);
    glPopMatrix();
    glPopAttrib();
}

void drawCheckerFloor() {
    const int halfSize = 24;
    const float tile = 1.0f;

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat floorSpecular[] = { 0.08f, 0.08f, 0.08f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, floorSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 12.0f);

    for (int x = -halfSize; x < halfSize; ++x) {
        for (int z = -halfSize; z < halfSize; ++z) {
            bool dark = ((x + z) & 1) != 0;
            if (dark) glColor3f(0.30f, 0.31f, 0.31f);
            else      glColor3f(0.55f, 0.56f, 0.55f);

            glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(x * tile, 0.0f, z * tile);
            glVertex3f((x + 1) * tile, 0.0f, z * tile);
            glVertex3f((x + 1) * tile, 0.0f, (z + 1) * tile);
            glVertex3f(x * tile, 0.0f, (z + 1) * tile);
            glEnd();
        }
    }

    glPopAttrib();
}

void drawRobotBlobShadow() {
    float sx = 1.15f;
    float sz = 0.70f;
    float alpha = 0.32f;

    switch (robot.targetForm) {
    case HUMANOID:
        sx = 0.95f; sz = 0.55f; alpha = 0.30f;
        break;
    case CAR:
        sx = 1.35f; sz = 1.15f; alpha = 0.35f;
        break;
    case BOAT:
        sx = 1.15f; sz = 1.85f; alpha = 0.28f;
        break;
    case PLANE:
        sx = 2.10f; sz = 1.40f; alpha = 0.26f;
        break;
    }

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glPushMatrix();
    glTranslatef(robot.posX, 0.018f, robot.posZ);
    glRotatef(robot.rotY, 0.0f, 1.0f, 0.0f);

    // Sombra del transformer
    glColor4f(0.0f, 0.0f, 0.0f, alpha);
    drawEllipseXZ(sx, sz, 80);

    glPopMatrix();
    glDepthMask(GL_TRUE);
    glPopAttrib();
}


void drawBitmapText(float x, float y, void* font, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) glutBitmapCharacter(font, c);
}

void drawOverlay() {
    float partX, partY, partZ;
    float colorR, colorG, colorB;
    robot.getSelectedPartPosition(partX, partY, partZ);
    robot.getSelectedPartColor(colorR, colorG, colorB);

    std::ostringstream posStream;
    posStream << std::fixed << std::setprecision(2)
        << "Coords robot=(" << robot.posX << ", " << robot.posY << ", " << robot.posZ
        << ") pieza=(" << partX << ", " << partY << ", " << partZ << ")";

    std::ostringstream colorStream;
    colorStream << std::fixed << std::setprecision(2)
        << "Color seleccionado RGB=(" << colorR << ", " << colorG << ", " << colorB << ")";

    std::string modeLine = std::string("Modo: ") + robot.getModeName() +
        " | Pieza: " + robot.getSelectedPartName();
    std::string controls1 = "Controles: [1] Robot | [2] Auto/Camion | [3] Barco | [4] Avion | [W]/[S] mover | [A]/[D] girar | [G] saludar | [T] disparar";
    std::string controls2 = "Clic izq. y arrastrar: Robot mueve extremidades | Avion gira helice | Auto/Camion abre/cierra puertas";
    std::string controls3 = "Seleccion: [Q] anterior | [E] siguiente | [C] cambiar color | [V] quitar color";

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glColor3f(0.08f, 0.08f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, (float)windowHeight);
    glVertex2f((float)windowWidth, (float)windowHeight);
    glVertex2f((float)windowWidth, (float)windowHeight - 112.0f);
    glVertex2f(0.0f, (float)windowHeight - 112.0f);
    glEnd();

    glColor3f(0.95f, 0.95f, 0.95f);
    drawBitmapText(12.0f, (float)windowHeight - 22.0f, GLUT_BITMAP_HELVETICA_18, modeLine);
    drawBitmapText(12.0f, (float)windowHeight - 42.0f, GLUT_BITMAP_HELVETICA_12, posStream.str());
    drawBitmapText(12.0f, (float)windowHeight - 58.0f, GLUT_BITMAP_HELVETICA_12, colorStream.str());
    drawBitmapText(12.0f, (float)windowHeight - 74.0f, GLUT_BITMAP_HELVETICA_12, controls1);
    drawBitmapText(12.0f, (float)windowHeight - 90.0f, GLUT_BITMAP_HELVETICA_12, controls2);
    drawBitmapText(12.0f, (float)windowHeight - 106.0f, GLUT_BITMAP_HELVETICA_12, controls3);

    glColor3f(colorR, colorG, colorB);
    glBegin(GL_QUADS);
    glVertex2f((float)windowWidth - 48.0f, (float)windowHeight - 22.0f);
    glVertex2f((float)windowWidth - 16.0f, (float)windowHeight - 22.0f);
    glVertex2f((float)windowWidth - 16.0f, (float)windowHeight - 54.0f);
    glVertex2f((float)windowWidth - 48.0f, (float)windowHeight - 54.0f);
    glEnd();

    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    for (int i = 0; i < 256; i++) keys[i] = false;

    glClearColor(0.42f, 0.72f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // Iluminacion fija de OpenGL: difusa + especular.
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat matSpecular[] = { 0.18f, 0.18f, 0.18f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 24.0f);

    // Atenuacion suave para que se note mas el punto de luz del sol.
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.018f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0015f);

    // Niebla muy suave: ayuda a integrar el piso con el cielo.
    GLfloat fogColor[] = { 0.42f, 0.72f, 1.0f, 1.0f };
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 22.0f);
    glFogf(GL_FOG_END, 48.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSkyBackground();
    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.apply();
    setupWorldLighting();

    drawSun();
    drawCheckerFloor();
    drawRobotBlobShadow();

    robot.draw();
    drawOverlay();
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
        case 'q': case 'Q': robot.selectPreviousPart(); break;
        case 'e': case 'E': robot.selectNextPart(); break;
        case '1': robot.setForm(HUMANOID); break;
        case '2': robot.setForm(CAR); break;
        case '3': robot.setForm(BOAT); break;
        case '4': robot.setForm(PLANE); break;
        case '5': robot.applySelectedColorPreset(0); break;
        case '6': robot.applySelectedColorPreset(1); break;
        case '7': robot.applySelectedColorPreset(2); break;
        case '8': robot.applySelectedColorPreset(3); break;
        case '9': robot.applySelectedColorPreset(4); break;
        case '0': robot.applySelectedColorPreset(5); break;
        case 'g': case 'G': robot.greet(); break;
        case 't': case 'T': robot.shoot(); break;
        case 'c': case 'C': robot.changeColor(); break;
        case 'v': case 'V': robot.clearColor(); break;
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

            // modo avion: arrastrar horizontalmente para girar la helice
            if (robot.targetForm == PLANE && robot.currentForm == PLANE &&
                robot.transformFactor >= 1.0f && robot.hitTestRobotBody(x, y)) {
                robot.isDraggingPropeller = true;
                isDragging = false;
            }
            // modo auto/camion: arrastrar horizontalmente para abrir/cerrar puertas
            else if (robot.targetForm == CAR && robot.currentForm == CAR &&
                robot.transformFactor >= 1.0f && robot.hitTestRobotBody(x, y)) {
                robot.isDraggingDoor = true;
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
            robot.isDraggingDoor = false;
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
    } else if (robot.isDraggingDoor) {
        robot.doorAngle += (float)dx * 0.6f;
        if (robot.doorAngle < 0.0f) robot.doorAngle = 0.0f;
        if (robot.doorAngle > 85.0f) robot.doorAngle = 85.0f;
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
    windowWidth = w;
    windowHeight = h;
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
    glutCreateWindow("Proyecto Transformer");

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
