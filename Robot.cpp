#include "Robot.h"
#include <cmath>
#include <iostream>

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

Robot::Robot() : posX(0), posY(0), posZ(0), rotY(0), currentForm(HUMANOID), targetForm(HUMANOID), walkCycle(0), wheelRotation(0), isMoving(false), transformFactor(1.0f), greetingTimer(0.0f) {}

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

void Robot::greet() {
    if (greetingTimer <= 0) {
        greetingTimer = 2.0f; // 2 seconds duration
    }
}

void Robot::update(float deltaTime) {
    if (transformFactor < 1.0f) {
        transformFactor += deltaTime * 1.5f;
        if (transformFactor > 1.0f) transformFactor = 1.0f;
    }

    if (greetingTimer > 0) {
        greetingTimer -= deltaTime;
        if (greetingTimer < 0) greetingTimer = 0;
    }

    if (isMoving) {
        if (targetForm == HUMANOID) {
            walkCycle += deltaTime * 20.0f;
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

void Robot::draw() {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0, 1, 0);

    for (int i = 0; i < 19; ++i) {
        drawPart(i, transformFactor);
    }

    glPopMatrix();
}

