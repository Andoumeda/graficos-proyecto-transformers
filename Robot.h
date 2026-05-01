#ifndef ROBOT_H
#define ROBOT_H

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

enum RobotForm { HUMANOID, CAR, BOAT, PLANE };

struct PartTransform {
    float tx, ty, tz;
    float rx, ry, rz;
    float sx, sy, sz;
    int shapeType; // 0: prisma, 1: cilindro, 2: esfera
    float r, g, b; // color
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
    float greetingTimer; // timer for greeting animation

    Robot();
    void draw();
    void update(float deltaTime);
    void setForm(RobotForm form);
    void turn(float angle);
    void moveForward(float distance);
    void greet(); // trigger greeting

private:
    void drawCube(float w, float h, float d);
    void drawCylinder(float radius, float h);
    void drawSphere(float r);

    PartTransform getPartTransform(int partIdx, RobotForm form);
    void drawPart(int partIdx, float factor);
    
    PartTransform interpolateTransform(const PartTransform& t1, const PartTransform& t2, float factor);
};

#endif
