#include "Robot.h"
#include <cmath>
#include <iostream>
#include <windows.h>

const PartTransform humanoidParts[19] = {
{0.000f, 2.121f, 0.000f, 0.000f, 0.000f, 0.000f, 1.000f, 0.805f, 0.606f, 0, 0.667f, 0.000f, 0.000f}, // 0: torso
{0.000f, 1.537f, 0.000f, 0.000f, 0.000f, 0.000f, 0.800f, 0.408f, 0.512f, 0, 0.667f, 0.000f, 0.000f}, // 1: cadera
{0.663f, 2.134f, 0.000f, 0.000f, 0.000f, 0.000f, 0.170f, 0.604f, 0.174f, 1, 0.000f, 0.667f, 0.000f}, // 2: brazo-der
{-0.672f, 2.134f, 0.000f, 0.000f, 0.000f, 0.000f, 0.170f, 0.604f, 0.174f, 1, 0.000f, 0.667f, 0.000f}, // 3: brazo-izq
{0.673f, 1.493f, 0.000f, 0.000f, 0.000f, 0.000f, 0.170f, 0.604f, 0.174f, 1, 0.000f, 0.667f, 0.000f}, // 4: antebrazo-der
{-0.697f, 1.493f, -0.002f, 0.000f, 0.000f, 0.000f, 0.170f, 0.604f, 0.174f, 1, 0.000f, 0.667f, 0.000f}, // 5: antebrazo-izq
{0.244f, 1.053f, 0.000f, 0.000f, 0.000f, 0.000f, 0.179f, 0.548f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 6: muslo-der
{-0.247f, 1.055f, 0.000f, 0.000f, 0.000f, 0.000f, 0.179f, 0.535f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 7: muslo-izq
{0.238f, 0.462f, 0.000f, 0.000f, 0.000f, 0.000f, 0.179f, 0.604f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 8: pierna-der
{-0.254f, 0.467f, 0.000f, 0.000f, 0.000f, 0.000f, 0.179f, 0.604f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 9: pierna-izq
{0.248f, 0.065f, 0.052f, 0.000f, 0.000f, 0.000f, 0.180f, 0.117f, 0.360f, 0, 0.000f, 0.000f, 0.667f}, // 10: pie-der
{-0.245f, 0.065f, 0.052f, 0.000f, 0.000f, 0.000f, 0.180f, 0.117f, 0.360f, 0, 0.000f, 0.000f, 0.667f}, // 11: pie-izq
{0.680f, 1.047f, 0.035f, 0.000f, 0.000f, 0.000f, 0.204f, 0.199f, 0.198f, 2, 1.000f, 1.000f, 1.000f}, // 12: mano-der
{-0.680f, 1.047f, 0.035f, 0.000f, 0.000f, 0.000f, 0.204f, 0.199f, 0.198f, 2, 1.000f, 1.000f, 1.000f}, // 13: mano-izq
{0.000f, 2.764f, 0.000f, 0.000f, 0.000f, 0.000f, 0.521f, 0.534f, 0.521f, 2, 0.800f, 0.800f, 0.800f}, // 14: cabeza
{-0.449f, 1.351f, 0.000f, 0.000f, 0.000f, -90.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}, // 15: rueda-del-izq
{0.454f, 1.351f, 0.000f, 0.000f, 0.000f, -90.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}, // 16: rueda-del-der
{0.534f, 2.471f, 0.000f, 90.000f, -90.000f, 0.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}, // 17: rueda-tras-der
{-0.526f, 2.471f, 0.000f, 90.000f, -90.000f, 0.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}  // 18: rueda-tras-izq
};

const PartTransform carParts[19] = {
{0.000f, 0.744f, 0.555f, 0.000f, 0.000f, 0.000f, 1.000f, 0.805f, 0.606f, 0, 0.667f, 0.000f, 0.000f}, // 0: torso
{0.000f, 0.545f, 1.126f, 0.000f, 0.000f, 0.000f, 0.800f, 0.408f, 0.512f, 0, 0.667f, 0.000f, 0.000f}, // 1: cadera
{0.456f, 1.098f, 0.234f, 0.000f, 0.000f, 0.000f, 0.129f, 1.020f, 0.131f, 1, 0.000f, 0.667f, 0.000f}, // 2: brazo-der
{-0.445f, 1.098f, 0.234f, 0.000f, 0.000f, 0.000f, 0.129f, 1.020f, 0.131f, 1, 0.000f, 0.667f, 0.000f}, // 3: brazo-izq
{0.173f, 0.404f, -0.337f, 90.000f, 0.000f, 0.000f, 0.301f, 1.147f, 0.055f, 1, 0.000f, 0.667f, 0.000f}, // 4: antebrazo-der
{-0.179f, 0.404f, -0.337f, 90.000f, 0.000f, 0.000f, 0.301f, 1.147f, 0.055f, 1, 0.000f, 0.667f, 0.000f}, // 5: antebrazo-izq
{0.450f, 0.465f, -0.033f, 90.000f, 0.000f, 0.000f, 0.179f, 0.554f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 6: muslo-der
{-0.447f, 0.465f, -0.033f, 90.000f, 0.000f, 0.000f, 0.179f, 0.554f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 7: muslo-izq
{0.451f, 0.465f, -0.610f, 90.000f, 0.000f, 0.000f, 0.179f, 0.597f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 8: pierna-der
{-0.446f, 0.465f, -0.610f, 90.000f, 0.000f, 0.000f, 0.179f, 0.597f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 9: pierna-izq
{0.254f, 0.480f, -0.952f, 0.000f, 90.000f, 90.000f, 0.180f, 0.094f, 0.480f, 0, 0.000f, 0.000f, 0.667f}, // 10: pie-der
{-0.255f, 0.480f, -0.949f, 0.000f, 90.000f, 90.000f, 0.180f, 0.094f, 0.480f, 0, 0.000f, 0.000f, 0.667f}, // 11: pie-izq
{0.221f, 0.515f, 1.366f, 0.000f, 0.000f, 0.000f, 0.204f, 0.199f, 0.198f, 2, 1.000f, 1.000f, 1.000f}, // 12: mano-der
{-0.220f, 0.515f, 1.366f, 0.000f, 0.000f, 0.000f, 0.204f, 0.199f, 0.198f, 2, 1.000f, 1.000f, 1.000f}, // 13: mano-izq
{0.000f, 0.663f, 0.232f, 0.000f, 0.000f, 0.000f, 0.521f, 0.534f, 0.521f, 2, 0.800f, 0.800f, 0.800f}, // 14: cabeza
{-0.420f, 0.218f, 0.979f, 0.000f, 0.000f, -90.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}, // 15: rueda-del-izq
{0.425f, 0.218f, 0.979f, 0.000f, 0.000f, -90.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}, // 16: rueda-del-der
{0.462f, 0.218f, -0.529f, 0.000f, 0.000f, -90.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}, // 17: rueda-tras-der
{-0.459f, 0.218f, -0.529f, 0.000f, 0.000f, -90.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}  // 18: rueda-tras-izq
};

const PartTransform boatParts[19] = {
{0.003f, 0.248f, -0.158f, 0.000f, 0.000f, 0.000f, 0.902f, 0.346f, 1.537f, 0, 0.667f, 0.000f, 0.000f}, // 0: torso
{0.000f, 0.545f, 0.903f, 0.000f, 0.000f, 0.000f, 0.600f, 0.354f, 0.802f, 0, 0.667f, 0.000f, 0.000f}, // 1: cadera
{0.001f, 1.008f, -0.204f, 0.000f, 0.000f, 0.000f, 0.123f, 1.195f, 0.115f, 1, 0.000f, 0.667f, 0.000f}, // 2: brazo-der
{0.003f, 0.887f, 1.368f, 45.000f, 0.000f, 0.000f, 0.167f, 0.596f, 0.167f, 1, 0.000f, 0.667f, 0.000f}, // 3: brazo-izq
{0.000f, 1.284f, -0.230f, 90.000f, 0.000f, -90.000f, 0.301f, 1.147f, 0.055f, 1, 0.000f, 0.667f, 0.000f}, // 4: antebrazo-der
{0.004f, 0.110f, 0.549f, 75.000f, 0.000f, 0.000f, 0.118f, 0.549f, 0.298f, 1, 0.000f, 0.667f, 0.000f}, // 5: antebrazo-izq
{0.458f, 0.465f, 0.219f, 90.000f, 0.000f, 0.000f, 0.142f, 0.714f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 6: muslo-der
{-0.460f, 0.465f, 0.219f, 90.000f, 0.000f, 0.000f, 0.142f, 0.714f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 7: muslo-izq
{0.458f, 0.465f, -0.534f, 90.000f, 0.000f, 0.000f, 0.142f, 0.774f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 8: pierna-der
{-0.460f, 0.465f, -0.534f, 90.000f, 0.000f, 0.000f, 0.142f, 0.774f, 0.272f, 1, 0.000f, 0.667f, 0.000f}, // 9: pierna-izq
{0.254f, 0.589f, -1.200f, 0.000f, 75.000f, -90.000f, 0.180f, 0.094f, 0.480f, 0, 0.000f, 0.000f, 0.667f}, // 10: pie-der
{-0.249f, 0.593f, -1.200f, 180.000f, 105.000f, 90.000f, 0.180f, 0.094f, 0.480f, 0, 0.000f, 0.000f, 0.667f}, // 11: pie-izq
{0.004f, 1.118f, 1.586f, 0.000f, 0.000f, 0.000f, 0.204f, 0.199f, 0.198f, 2, 1.000f, 1.000f, 1.000f}, // 12: mano-der
{0.003f, 1.676f, -0.206f, 0.000f, 0.000f, 0.000f, 0.204f, 0.199f, 0.198f, 2, 1.000f, 1.000f, 1.000f}, // 13: mano-izq
{0.022f, 0.477f, -1.025f, 0.000f, 0.000f, 0.000f, 0.797f, 0.417f, 0.418f, 2, 0.800f, 0.800f, 0.800f}, // 14: cabeza
{-0.521f, 0.664f, 0.185f, 0.000f, 0.000f, -90.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}, // 15: rueda-del-izq
{0.522f, 0.664f, 0.185f, 0.000f, 0.000f, -90.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}, // 16: rueda-del-der
{0.520f, 0.658f, -0.557f, 0.000f, 0.000f, -90.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}, // 17: rueda-tras-der
{-0.521f, 0.658f, -0.557f, 0.000f, 0.000f, -90.000f, 0.401f, 0.097f, 0.400f, 1, 0.000f, 0.000f, 0.000f}  // 18: rueda-tras-izq
};

const PartTransform planeParts[19] = {
{0.000f, 0.800f, 0.000f, 90.000f, 0.000f, 0.000f, 0.450f, 2.200f, 0.450f, 0, 0.667f, 0.000f, 0.000f}, // 0: torso
{0.000f, 0.800f, 0.037f, 0.000f, 0.000f, 0.000f, 0.500f, 0.350f, 1.800f, 0, 0.667f, 0.000f, 0.000f}, // 1: cadera
{1.001f, 0.789f, -0.313f, 177.000f, -1.000f, 90.000f, -0.125f, 1.575f, 0.565f, 1, 0.000f, 0.667f, 0.000f}, // 2: brazo-der
{-1.001f, 0.789f, -0.313f, 177.000f, 1.000f, -90.000f, -0.125f, 1.575f, 0.565f, 1, 0.000f, 0.667f, 0.000f}, // 3: brazo-izq
{1.671f, 0.764f, -0.324f, 0.000f, 0.000f, 0.000f, 0.150f, 0.450f, 0.150f, 1, 0.000f, 0.667f, 0.000f}, // 4: antebrazo-der
{-1.671f, 0.764f, -0.324f, 0.000f, 0.000f, 0.000f, 0.150f, 0.450f, 0.150f, 1, 0.000f, 0.667f, 0.000f}, // 5: antebrazo-izq
{0.412f, 0.850f, 0.950f, 0.000f, 90.000f, 90.000f, 0.059f, 0.199f, 0.745f, 1, 0.000f, 0.667f, 0.000f}, // 6: muslo-der
{-0.412f, 0.850f, 0.950f, 0.000f, 90.000f, 90.000f, 0.059f, 0.199f, 0.745f, 1, 0.000f, 0.667f, 0.000f}, // 7: muslo-izq
{0.000f, 1.150f, 1.000f, 0.000f, 0.000f, 0.000f, 0.080f, 0.650f, 0.350f, 1, 0.000f, 0.667f, 0.000f}, // 8: pierna-der
{0.000f, 0.800f, 1.100f, 90.000f, 0.000f, 0.000f, 0.300f, 0.150f, 0.300f, 1, 0.667f, 0.000f, 0.000f}, // 9: pierna-izq (torso-like color)
{0.000f, 0.574f, -0.900f, 0.000f, 0.000f, 0.000f, 0.400f, 0.060f, 0.250f, 0, 0.000f, 0.000f, 0.667f}, // 10: prisma (pie-der)
{0.000f, 0.574f, 0.800f, 0.000f, 0.000f, 0.000f, 0.400f, 0.060f, 0.250f, 0, 0.000f, 0.000f, 0.667f}, // 11: prisma_1 (pie-izq)
{1.671f, 0.580f, -0.332f, 90.000f, 0.000f, 0.000f, 0.120f, 0.250f, 0.120f, 2, 1.000f, 1.000f, 1.000f}, // 12: mano-der
{-1.671f, 0.580f, -0.332f, 90.000f, 0.000f, 0.000f, 0.120f, 0.250f, 0.120f, 2, 1.000f, 1.000f, 1.000f}, // 13: mano-izq
{0.000f, 0.799f, -1.134f, 0.000f, 0.000f, 0.000f, 0.395f, 0.399f, 0.600f, 2, 0.800f, 0.800f, 0.800f}, // 14: cabeza
{-0.200f, 0.480f, -0.900f, 0.000f, 0.000f, -90.000f, 0.180f, 0.080f, 0.180f, 1, 0.000f, 0.000f, 0.000f}, // 15: rueda-del-izq
{0.200f, 0.480f, -0.900f, 0.000f, 0.000f, -90.000f, 0.180f, 0.080f, 0.180f, 1, 0.000f, 0.000f, 0.000f}, // 16: rueda-del-der
{0.200f, 0.516f, 0.800f, 0.000f, 0.000f, -90.000f, 0.180f, 0.080f, 0.180f, 1, 0.000f, 0.000f, 0.000f}, // 17: rueda-tras-der
{-0.200f, 0.516f, 0.800f, 0.000f, 0.000f, -90.000f, 0.180f, 0.080f, 0.180f, 1, 0.000f, 0.000f, 0.000f}  // 18: rueda-tras-izq
};

Robot::Robot() : posX(0), posY(0), posZ(0), rotY(0), currentForm(HUMANOID), targetForm(HUMANOID), walkCycle(0), wheelRotation(0), isMoving(false), transformFactor(1.0f), greetingTimer(0.0f), shootingTimer(0.0f), moveState(IDLE), isEditMode(false), shoulderRightAngle(0), shoulderLeftAngle(0), elbowRightAngle(0), elbowLeftAngle(0), hipRightAngle(0), hipLeftAngle(0), kneeRightAngle(0), kneeLeftAngle(0), dragJointIndex(-1), isDraggingJoint(false) {
    for (int i = 0; i < 8; i++) controlPoints[i].visible = false;
}

void Robot::drawCube(float w, float h, float d) {
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void Robot::drawCylinder(float r, float h) {
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, r, r, h, 20, 20);
    glPushMatrix();
    gluDisk(quad, 0, r, 20, 1);
    glTranslatef(0, 0, h);
    gluDisk(quad, 0, r, 20, 1);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void Robot::drawSphere(float r) {
    glutSolidSphere(r, 20, 20);
}

void Robot::playGreetingSound() {
    Beep(660, 90);
    Beep(880, 110);
    Beep(740, 90);
}

void Robot::playShotSound() {
    Beep(1200, 45);
    Beep(420, 70);
}

void Robot::drawPrimitiveAtOrigin(const PartTransform& t) {
    glColor3f(t.r, t.g, t.b);

    if (t.shapeType == 0) {
        drawCube(t.sx, t.sy, t.sz);
    } else if (t.shapeType == 1) {
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glScalef(t.sx, t.sz, t.sy);
        glTranslatef(0.0f, 0.0f, -0.5f);
        drawCylinder(0.5f, 1.0f);
        glPopMatrix();
    } else if (t.shapeType == 2) {
        glPushMatrix();
        glScalef(t.sx, t.sy, t.sz);
        drawSphere(0.5f);
        glPopMatrix();
    }
}

void Robot::setForm(RobotForm form) {
    if (form != targetForm) {
        currentForm = targetForm;
        targetForm = form;
        transformFactor = 0.0f;
    }
}

void Robot::turn(float angle) {
    rotY += angle;
}

void Robot::moveForward(float distance) {
    float rad = rotY * 3.14159f / 180.0f;
    posX += sin(rad) * distance;
    posZ += cos(rad) * distance;
    isMoving = true;
    greetingTimer = 0; // Cancel greeting if moving
}

void Robot::toggleForward() {
    if (moveState == MOVING_FORWARD) {
        moveState = IDLE;
    } else {
        moveState = MOVING_FORWARD;
        if (isEditMode) toggleEditMode();
    }
}

void Robot::toggleBackward() {
    if (moveState == MOVING_BACKWARD) {
        moveState = IDLE;
    } else {
        moveState = MOVING_BACKWARD;
        if (isEditMode) toggleEditMode();
    }
}

void Robot::greet() {
    if (greetingTimer <= 0) {
        greetingTimer = 2.0f; // 2 seconds duration
        playGreetingSound();
    }
}

void Robot::shoot() {
    shootingTimer = 0.45f;
    playShotSound();
}

void Robot::update(float deltaTime) {
    if (transformFactor < 1.0f) {
        transformFactor += deltaTime * 1.5f;
        if (transformFactor > 1.0f) transformFactor = 1.0f;
    }

    if (transformFactor >= 1.0f && currentForm != targetForm) {
        currentForm = targetForm;
    }

    if (greetingTimer > 0) {
        greetingTimer -= deltaTime;
        if (greetingTimer < 0) greetingTimer = 0;
    }

    if (shootingTimer > 0) {
        shootingTimer -= deltaTime;
        if (shootingTimer < 0) shootingTimer = 0;
    }

    if (isMoving) {
        if (targetForm == HUMANOID) {
            walkCycle += deltaTime * 20.0f;
        } else {
            wheelRotation += deltaTime * 420.0f;
            if (wheelRotation > 360.0f) wheelRotation -= 360.0f;
            walkCycle += deltaTime * 8.0f;
        }
    }
    isMoving = false;
}

PartTransform Robot::getPartTransform(int partIdx, RobotForm form) {
    if (form == HUMANOID) return humanoidParts[partIdx];
    if (form == CAR) return carParts[partIdx];
    if (form == BOAT) return boatParts[partIdx];
    
    // Para el avión, invertimos el eje Z para que el frente sea atrás
    PartTransform t = planeParts[partIdx];
    t.tz = -t.tz; 
    t.ry += 180.0f; // Rotamos 180 grados para que mire hacia el otro lado
    return t;
}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

float lerpAngle(float a, float b, float f) {
    float diff = fmod(b - a, 360.0f);
    if (diff < -180.0f) diff += 360.0f;
    if (diff > 180.0f) diff -= 360.0f;
    return a + f * diff;
}

PartTransform Robot::interpolateTransform(const PartTransform& t1, const PartTransform& t2, float factor) {
    PartTransform res;
    res.tx = lerp(t1.tx, t2.tx, factor);
    res.ty = lerp(t1.ty, t2.ty, factor);
    res.tz = lerp(t1.tz, t2.tz, factor);
    res.rx = lerpAngle(t1.rx, t2.rx, factor);
    res.ry = lerpAngle(t1.ry, t2.ry, factor);
    res.rz = lerpAngle(t1.rz, t2.rz, factor);
    res.sx = lerp(t1.sx, t2.sx, factor);
    res.sy = lerp(t1.sy, t2.sy, factor);
    res.sz = lerp(t1.sz, t2.sz, factor);
    res.r = lerp(t1.r, t2.r, factor);
    res.g = lerp(t1.g, t2.g, factor);
    res.b = lerp(t1.b, t2.b, factor);
    res.shapeType = t2.shapeType; 
    return res;
}

void Robot::drawPart(int partIdx, float factor) {
    PartTransform curr = getPartTransform(partIdx, currentForm);
    PartTransform target = getPartTransform(partIdx, targetForm);
    PartTransform t = interpolateTransform(curr, target, factor);

    float legSwing = (targetForm == HUMANOID) ? sin(walkCycle) * 20.0f : 0;
    
    // Solo aplicar animaciones de movimiento en modo HUMANOIDE
    if (targetForm == HUMANOID) {
        // Lado derecho: 2(brazo), 4(antebrazo), 6(muslo), 8(pierna), 10(pie)
        if (partIdx == 2 || partIdx == 4 || partIdx == 6 || partIdx == 8 || partIdx == 10) {
            t.rx += legSwing;
        }
        // Lado izquierdo: 3(brazo), 5(antebrazo), 7(muslo), 9(pierna), 11(pie)
        if (partIdx == 3 || partIdx == 5 || partIdx == 7 || partIdx == 9 || partIdx == 11) {
            t.rx -= legSwing;
        }
    }

    // Greeting animation
    if (greetingTimer > 0) {
        if (targetForm == HUMANOID) {
            // Brazo derecho saluda (indice 2, 4)
            if (partIdx == 2 || partIdx == 4) {
                float wave = sin(greetingTimer * 15.0f) * 30.0f;
                t.rz += wave;
                t.rx -= 140.0f;
            }
        } else {
            // Luces parpadeando (indice 12 y 13 manos)
            if (partIdx == 12 || partIdx == 13) {
                int blink = (int)(greetingTimer * 4.0f) % 2;
                if (blink == 0) {
                    t.r = 1.0f; t.g = 1.0f; t.b = 0.0f;
                    t.sx *= 1.5f; t.sy *= 1.5f; t.sz *= 1.5f;
                }
            }
        }
    }

    glPushMatrix();
    glTranslatef(t.tx, t.ty, t.tz);
    glRotatef(t.ry, 0, 1, 0);
    glRotatef(t.rx, 1, 0, 0);
    glRotatef(t.rz, 0, 0, 1);
    
    glColor3f(t.r, t.g, t.b);

    if (t.shapeType == 0) {
        // Prisma: escala directa en X, Y, Z
        drawCube(t.sx, t.sy, t.sz);
    } else if (t.shapeType == 1) {
        // Cilindro: JSON convention -> Y = altura, XZ = radios/diametros
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);  // Local Z es ahora Global Y
        glScalef(t.sx, t.sz, t.sy);            // Local X=X, Local Y=Z, Local Z=Y
        glTranslatef(0.0f, 0.0f, -0.5f);       // Centrar en su eje
        drawCylinder(0.5f, 1.0f);              // Radio 0.5 (diametro 1.0), altura 1.0
        glPopMatrix();
    } else if (t.shapeType == 2) {
        // Esfera: escala uniforme
        glScalef(t.sx, t.sy, t.sz);
        drawSphere(0.5f);
    }

    glPopMatrix();
}

void Robot::drawLocalPart(const PartTransform& parent, int partIdx, RobotForm form, float extraRx, float extraRy, float extraRz) {
    PartTransform t = getPartTransform(partIdx, form);

    glPushMatrix();
    glTranslatef(t.tx - parent.tx, t.ty - parent.ty, t.tz - parent.tz);
    glRotatef(t.ry + extraRy, 0, 1, 0);
    glRotatef(t.rx + extraRx, 1, 0, 0);
    glRotatef(t.rz + extraRz, 0, 0, 1);
    drawPrimitiveAtOrigin(t);
    glPopMatrix();
}

void Robot::drawHierarchicalArm(bool rightSide, float shoulderSwing, float wave, float userShoulderAngle, float userElbowAngle) {
    int upperIdx = rightSide ? 2 : 3;
    int lowerIdx = rightSide ? 4 : 5;
    int handIdx = rightSide ? 12 : 13;
    const PartTransform upper = humanoidParts[upperIdx];
    const PartTransform lower = humanoidParts[lowerIdx];
    const PartTransform hand = humanoidParts[handIdx];

    float side = rightSide ? 1.0f : -1.0f;
    float shoulderX = side * 0.66f;
    float shoulderY = 2.42f;
    float elbowBend = rightSide ? -8.0f : 8.0f;

    if (isEditMode) {
        shoulderSwing = userShoulderAngle;
        elbowBend = userElbowAngle;
    } else if (rightSide && greetingTimer > 0.0f) {
        shoulderSwing = -120.0f;
        elbowBend = -55.0f + wave;
    }

    glPushMatrix();
    glTranslatef(shoulderX, shoulderY, 0.0f);
    glRotatef(shoulderSwing, 1.0f, 0.0f, 0.0f);
    glRotatef(side * 8.0f, 0.0f, 0.0f, 1.0f);

    if (isEditMode) {
        GLint vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
        GLdouble mv[16], pj[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, mv);
        glGetDoublev(GL_PROJECTION_MATRIX, pj);
        GLdouble sx, sy, sz;
        gluProject(0, 0, 0, mv, pj, vp, &sx, &sy, &sz);
        int idx = rightSide ? 0 : 1;
        controlPoints[idx].screenX = (float)sx;
        controlPoints[idx].screenY = (float)(vp[3] - sy);
        controlPoints[idx].visible = true;

        glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 0.0f);
        glutSolidSphere(0.08, 12, 12);
        glPopAttrib();
    }

    glPushMatrix();
    glTranslatef(0.0f, -upper.sy * 0.5f, 0.0f);
    drawPrimitiveAtOrigin(upper);
    glPopMatrix();

    glTranslatef(0.0f, -upper.sy, 0.0f);
    glRotatef(elbowBend, 1.0f, 0.0f, 0.0f);

    if (isEditMode) {
        GLint vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
        GLdouble mv[16], pj[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, mv);
        glGetDoublev(GL_PROJECTION_MATRIX, pj);
        GLdouble sx, sy, sz;
        gluProject(0, 0, 0, mv, pj, vp, &sx, &sy, &sz);
        int idx = rightSide ? 2 : 3;
        controlPoints[idx].screenX = (float)sx;
        controlPoints[idx].screenY = (float)(vp[3] - sy);
        controlPoints[idx].visible = true;

        glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
        glDisable(GL_LIGHTING);
        glColor3f(0.0f, 1.0f, 1.0f);
        glutSolidSphere(0.08, 12, 12);
        glPopAttrib();
    }

    glPushMatrix();
    glTranslatef(0.0f, -lower.sy * 0.5f, 0.0f);
    drawPrimitiveAtOrigin(lower);
    glPopMatrix();

    glTranslatef(0.0f, -lower.sy - 0.08f, 0.0f);
    drawPrimitiveAtOrigin(hand);
    glPopMatrix();
}

void Robot::drawHierarchicalVehicle(RobotForm form) {
    PartTransform root = getPartTransform(1, form); // cadera como nodo raiz del vehiculo
    float bob = 0.0f;
    float roll = 0.0f;
    float pitch = 0.0f;

    if (form == CAR) {
        bob = fabs(sin(walkCycle)) * 0.025f;
        pitch = sin(walkCycle * 0.7f) * 1.5f;
    } else if (form == BOAT) {
        bob = sin(walkCycle) * 0.06f;
        roll = sin(walkCycle * 0.8f) * 3.5f;
        pitch = cos(walkCycle * 0.6f) * 2.0f;
    } else if (form == PLANE) {
        bob = sin(walkCycle * 0.9f) * 0.05f;
        roll = sin(walkCycle * 0.5f) * 4.0f;
        pitch = 4.0f + cos(walkCycle * 0.4f) * 1.5f;
    }

    glPushMatrix();
    glTranslatef(root.tx, root.ty + bob, root.tz);
    glRotatef(root.ry, 0.0f, 1.0f, 0.0f);
    glRotatef(root.rx + pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(root.rz + roll, 0.0f, 0.0f, 1.0f);

    drawPrimitiveAtOrigin(root);

    for (int i = 0; i < 19; ++i) {
        if (i == 1) continue;

        float extraRx = 0.0f;
        float extraRy = 0.0f;
        float extraRz = 0.0f;

        if (i >= 15 && i <= 18) {
            extraRx = wheelRotation;
        }

        if (form == BOAT && (i == 4 || i == 5)) {
            extraRy = sin(walkCycle * 2.0f) * 18.0f;
        }

        if (form == PLANE && (i == 2 || i == 3)) {
            extraRz = sin(walkCycle * 1.5f) * 5.0f;
        }

        if (form == PLANE && (i == 12 || i == 13)) {
            extraRx = wheelRotation * 2.0f;
        }

        drawLocalPart(root, i, form, extraRx, extraRy, extraRz);
    }

    glPopMatrix();
}

void Robot::drawShotEffect() {
    if (shootingTimer <= 0.0f) return;

    float progress = 1.0f - (shootingTimer / 0.45f);
    float beamLength = 1.2f + progress * 3.0f;
    float flashScale = 1.0f + sin(progress * 3.14159f) * 1.4f;

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(0.0f, targetForm == HUMANOID ? 1.55f : 0.75f, 1.25f);

    glColor3f(1.0f, 0.85f, 0.05f);
    glPushMatrix();
    glScalef(0.18f * flashScale, 0.18f * flashScale, 0.18f * flashScale);
    glutSolidSphere(1.0f, 20, 20);
    glPopMatrix();

    glColor3f(1.0f, 0.15f, 0.05f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, beamLength * 0.5f);
    drawCylinder(0.045f, beamLength);
    glPopMatrix();

    glColor3f(1.0f, 0.95f, 0.25f);
    glTranslatef(0.0f, 0.0f, beamLength);
    glutSolidSphere(0.12f, 16, 16);

    glPopMatrix();
    glPopAttrib();
}

void Robot::drawHierarchicalLeg(bool rightSide, float hipSwing, float kneeBend, float userHipAngle, float userKneeAngle) {
    int thighIdx = rightSide ? 6 : 7;
    int shinIdx = rightSide ? 8 : 9;
    int footIdx = rightSide ? 10 : 11;
    const PartTransform thigh = humanoidParts[thighIdx];
    const PartTransform shin = humanoidParts[shinIdx];
    const PartTransform foot = humanoidParts[footIdx];

    float side = rightSide ? 1.0f : -1.0f;
    float hipX = side * 0.245f;
    float hipY = 1.31f;

    if (isEditMode) {
        hipSwing = userHipAngle;
        kneeBend = userKneeAngle;
    }

    glPushMatrix();
    glTranslatef(hipX, hipY, 0.0f);
    glRotatef(hipSwing, 1.0f, 0.0f, 0.0f);

    if (isEditMode) {
        GLint vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
        GLdouble mv[16], pj[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, mv);
        glGetDoublev(GL_PROJECTION_MATRIX, pj);
        GLdouble sx, sy, sz;
        gluProject(0, 0, 0, mv, pj, vp, &sx, &sy, &sz);
        int idx = rightSide ? 4 : 5;
        controlPoints[idx].screenX = (float)sx;
        controlPoints[idx].screenY = (float)(vp[3] - sy);
        controlPoints[idx].visible = true;

        glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 0.0f);
        glutSolidSphere(0.08, 12, 12);
        glPopAttrib();
    }

    glPushMatrix();
    glTranslatef(0.0f, -thigh.sy * 0.5f, 0.0f);
    drawPrimitiveAtOrigin(thigh);
    glPopMatrix();

    glTranslatef(0.0f, -thigh.sy, 0.0f);
    glRotatef(kneeBend, 1.0f, 0.0f, 0.0f);

    if (isEditMode) {
        GLint vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
        GLdouble mv[16], pj[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, mv);
        glGetDoublev(GL_PROJECTION_MATRIX, pj);
        GLdouble sx, sy, sz;
        gluProject(0, 0, 0, mv, pj, vp, &sx, &sy, &sz);
        int idx = rightSide ? 6 : 7;
        controlPoints[idx].screenX = (float)sx;
        controlPoints[idx].screenY = (float)(vp[3] - sy);
        controlPoints[idx].visible = true;

        glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
        glDisable(GL_LIGHTING);
        glColor3f(0.0f, 1.0f, 1.0f);
        glutSolidSphere(0.08, 12, 12);
        glPopAttrib();
    }

    glPushMatrix();
    glTranslatef(0.0f, -shin.sy * 0.5f, 0.0f);
    drawPrimitiveAtOrigin(shin);
    glPopMatrix();

    glTranslatef(0.0f, -shin.sy - 0.02f, 0.08f);
    glRotatef(-hipSwing * 0.35f, 1.0f, 0.0f, 0.0f);
    drawPrimitiveAtOrigin(foot);
    glPopMatrix();
}

void Robot::drawHierarchicalHumanoid() {
    float swing = sin(walkCycle) * 28.0f;
    float rightKnee = (swing < 0.0f) ? -swing * 1.35f : 4.0f;
    float leftKnee = (swing > 0.0f) ? swing * 1.35f : 4.0f;
    float bodyBob = fabs(sin(walkCycle)) * 0.035f;
    float wave = (greetingTimer > 0.0f) ? sin(greetingTimer * 15.0f) * 35.0f : 0.0f;

    if (isEditMode) {
        swing = 0;
        rightKnee = 0;
        leftKnee = 0;
        bodyBob = 0;
        wave = 0;
        for (int i = 0; i < 8; i++) controlPoints[i].visible = false;
    }

    glPushMatrix();
    glTranslatef(0.0f, bodyBob, 0.0f);

    glPushMatrix();
    glTranslatef(humanoidParts[1].tx, humanoidParts[1].ty, humanoidParts[1].tz);
    drawPrimitiveAtOrigin(humanoidParts[1]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(humanoidParts[0].tx, humanoidParts[0].ty, humanoidParts[0].tz);
    drawPrimitiveAtOrigin(humanoidParts[0]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(humanoidParts[14].tx, humanoidParts[14].ty, humanoidParts[14].tz);
    drawPrimitiveAtOrigin(humanoidParts[14]);
    glPopMatrix();

    drawHierarchicalArm(true, -swing * 0.75f, wave, shoulderRightAngle, elbowRightAngle);
    drawHierarchicalArm(false, swing * 0.75f, 0.0f, shoulderLeftAngle, elbowLeftAngle);
    drawHierarchicalLeg(true, swing, rightKnee, hipRightAngle, kneeRightAngle);
    drawHierarchicalLeg(false, -swing, leftKnee, hipLeftAngle, kneeLeftAngle);

    for (int i = 15; i < 19; ++i) {
        glPushMatrix();
        glTranslatef(humanoidParts[i].tx, humanoidParts[i].ty, humanoidParts[i].tz);
        glRotatef(humanoidParts[i].ry, 0, 1, 0);
        glRotatef(humanoidParts[i].rx, 1, 0, 0);
        glRotatef(humanoidParts[i].rz, 0, 0, 1);
        drawPrimitiveAtOrigin(humanoidParts[i]);
        glPopMatrix();
    }

    glPopMatrix();
}

void Robot::draw() {
    glGetDoublev(GL_MODELVIEW_MATRIX, cachedModelView);
    glGetDoublev(GL_PROJECTION_MATRIX, cachedProjection);
    glGetIntegerv(GL_VIEWPORT, cachedViewport);

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0, 1, 0);

    if (targetForm == HUMANOID && currentForm == HUMANOID && transformFactor >= 1.0f) {
        drawHierarchicalHumanoid();
    } else if (targetForm != HUMANOID && currentForm == targetForm && transformFactor >= 1.0f) {
        drawHierarchicalVehicle(targetForm);
    } else {
        for (int i = 0; i < 19; ++i) {
            drawPart(i, transformFactor);
        }
    }

    drawShotEffect();

    glPopMatrix();
}

bool Robot::canEdit() const {
    return targetForm == HUMANOID && currentForm == HUMANOID &&
           transformFactor >= 1.0f && moveState == IDLE && greetingTimer <= 0.0f;
}

void Robot::toggleEditMode() {
    if (!canEdit() && !isEditMode) return;
    isEditMode = !isEditMode;
    if (!isEditMode) {
        shoulderRightAngle = shoulderLeftAngle = 0;
        elbowRightAngle = elbowLeftAngle = 0;
        hipRightAngle = hipLeftAngle = 0;
        kneeRightAngle = kneeLeftAngle = 0;
        for (int i = 0; i < 8; i++) controlPoints[i].visible = false;
    } else {
        greetingTimer = 0;
    }
    dragJointIndex = -1;
    isDraggingJoint = false;
}

int Robot::hitTestControlPoint(int sx, int sy) const {
    int closest = -1;
    float minDist = 25.0f;
    for (int i = 0; i < 8; i++) {
        if (!controlPoints[i].visible) continue;
        float dx = (float)sx - controlPoints[i].screenX;
        float dy = (float)sy - controlPoints[i].screenY;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < minDist) {
            minDist = dist;
            closest = i;
        }
    }
    return closest;
}

bool Robot::hitTestRobotBody(int sx, int sy) const {
    GLdouble wx, wy, wz;
    gluProject(posX, posY + 1.5, posZ, cachedModelView, cachedProjection, cachedViewport, &wx, &wy, &wz);
    float cx = (float)wx;
    float cy = (float)(cachedViewport[3] - wy);
    float dx = (float)sx - cx;
    float dy = (float)sy - cy;
    return (dx * dx + dy * dy) < 60.0f * 60.0f;
}

void Robot::startDragJoint(int index) {
    dragJointIndex = index;
    isDraggingJoint = true;
}

void Robot::dragJoint(float dx) {
    if (dragJointIndex < 0 || dragJointIndex > 7) return;
    float angleDelta = dx * 0.5f;
    float* angles[] = {
        &shoulderRightAngle, &shoulderLeftAngle,
        &elbowRightAngle, &elbowLeftAngle,
        &hipRightAngle, &hipLeftAngle,
        &kneeRightAngle, &kneeLeftAngle
    };
    *angles[dragJointIndex] += angleDelta;
    float lo[] = { -180, -180, -150, -150, -120, -120, -50, -50 };
    float hi[] = {  180,  180,   50,   50,  120,  120, 150, 150 };
    if (*angles[dragJointIndex] < lo[dragJointIndex]) *angles[dragJointIndex] = lo[dragJointIndex];
    if (*angles[dragJointIndex] > hi[dragJointIndex]) *angles[dragJointIndex] = hi[dragJointIndex];
}

void Robot::endDragJoint() {
    dragJointIndex = -1;
    isDraggingJoint = false;
}
