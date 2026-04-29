#ifndef ROBOT_H
#define ROBOT_H

#include <GL/glew.h>
#include <GL/freeglut.h>

enum RobotForm { HUMANOID, CAR, PLANE, BOAT };

struct PartTransform {
    float tx, ty, tz;
    float rx, ry, rz;
    float sx, sy, sz;
};

class Robot {
public:
    float posX, posY, posZ;
    float rotY;
    RobotForm currentForm;
    RobotForm targetForm;
    
    // Animation states
    float walkCycle;
    float wheelRotation;
    bool isMoving;
    float transformFactor; // 0.0 to 1.0
    float greetingTimer; // New: timer for greeting animation

    Robot();
    void draw();
    void update(float deltaTime);
    void setForm(RobotForm form);
    void turn(float angle);
    void moveForward(float distance);
    void greet(); // New: trigger greeting


private:
    void drawCube(float w, float h, float d);
    void drawCylinder(float r, float h);
    void drawWheel(float r, float w);
    void drawLights();

    PartTransform getPartTransform(int partIdx, RobotForm form);
    void drawPart(int partIdx, float factor);
};

#endif
