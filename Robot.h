#ifndef ROBOT_H
#define ROBOT_H

#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

enum RobotForm { HUMANOID, CAR, BOAT, PLANE };
enum MoveState { IDLE, MOVING_FORWARD, MOVING_BACKWARD };

struct ControlPoint {
    float screenX, screenY;
    bool visible;
};

struct PartTransform {
    float tx, ty, tz;
    float rx, ry, rz;
    float sx, sy, sz;
    int shapeType; // 0: prisma, 1: cilindro, 2: esfera
    float r, g, b;
};

struct ColorPreset {
    float r, g, b;
};

class Robot {
public:
    float posX, posY, posZ;
    float rotY;
    RobotForm currentForm;
    RobotForm targetForm;
    
    float walkCycle;
    float wheelRotation;
    bool isMoving;
    float transformFactor;
    MoveState moveState;
    float greetingTimer; // duracion del saludo
    float shootingTimer; // duracion del disparo
    float propellerAngle; // angulo de la helice en modo avion
    bool isDraggingPropeller; // true si el usuario arrastra la helice
    float doorAngle;
    bool isDraggingDoor;

    bool isEditMode;
    float shoulderRightAngle, shoulderLeftAngle;
    float elbowRightAngle, elbowLeftAngle;
    float hipRightAngle, hipLeftAngle;
    float kneeRightAngle, kneeLeftAngle;
    ControlPoint controlPoints[8];
    int dragJointIndex;
    bool isDraggingJoint;
    double cachedModelView[16];
    double cachedProjection[16];
    int cachedViewport[4];
    int selectedPartIndex;
    bool hasCustomColor[19];
    ColorPreset customColors[19];

    Robot();
    void draw();
    void update(float deltaTime);
    void setForm(RobotForm form);
    void turn(float angle);
    void moveForward(float distance);
    void toggleForward();
    void toggleBackward();
    void greet();
    void shoot();
    bool canEdit() const;
    void toggleEditMode();
    int hitTestControlPoint(int sx, int sy) const;
    bool hitTestRobotBody(int sx, int sy) const;
    void startDragJoint(int index);
    void dragJoint(float dx);
    void endDragJoint();
    void selectNextPart();
    void selectPreviousPart();
    void applySelectedColorPreset(int presetIndex);
    const char* getModeName() const;
    const char* getSelectedPartName() const;
    void getSelectedPartColor(float& r, float& g, float& b) const;
    void getSelectedPartPosition(float& x, float& y, float& z) const;
    void changeColor();

private:
    void drawCube(float w, float h, float d);
    void drawCylinder(float radius, float h);
    void drawSphere(float r);
    void drawPrimitiveAtOrigin(const PartTransform& t);
    void drawLocalPart(const PartTransform& parent, int partIdx, RobotForm form, float extraRx = 0.0f, float extraRy = 0.0f, float extraRz = 0.0f);
    void drawCarDoor(const PartTransform& parent, int partIdx);
    void drawHierarchicalHumanoid();
    void drawHierarchicalVehicle(RobotForm form);
    void drawHierarchicalArm(bool rightSide, float shoulderSwing, float wave, float userShoulderAngle, float userElbowAngle);
    void drawHierarchicalLeg(bool rightSide, float hipSwing, float kneeBend, float userHipAngle, float userKneeAngle);
    void drawShotEffect();
    void playGreetingSound();
    void playShotSound();
    bool isPartSelected(int partIdx) const;
    void applyPartVisualState(PartTransform& t, int partIdx) const;

    PartTransform getPartTransform(int partIdx, RobotForm form);
    void drawPart(int partIdx, float factor);
    
    PartTransform interpolateTransform(const PartTransform& t1, const PartTransform& t2, float factor) const;
};

#endif
