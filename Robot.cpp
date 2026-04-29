#include "Robot.h"
#include <cmath>

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

void Robot::drawWheel(float r, float w) {
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glTranslatef(0, 0, -w / 2.0f);
    drawCylinder(r, w);
    glPopMatrix();
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
        } else if (targetForm == CAR) {
            wheelRotation += deltaTime * 500.0f;
        }
    }
    isMoving = false;
}

PartTransform Robot::getPartTransform(int partIdx, RobotForm form) {
    PartTransform t = {0,0,0, 0,0,0, 1,1,1};
    float legSwing = (form == HUMANOID) ? sin(walkCycle) * 20.0f : 0;
    
    // Greeting animation for humanoid
    float rightArmGreet = 0;
    if (form == HUMANOID && greetingTimer > 0) {
        rightArmGreet = 140.0f + sin(greetingTimer * 10.0f) * 20.0f;
    }

    switch (form) {
    case HUMANOID:
        switch(partIdx) {
            case 0: t.ty = 2.5f; t.sx=1.5f; t.sy=2.0f; t.sz=0.8f; break;
            case 1: t.ty = 3.8f; t.sx=0.6f; t.sy=0.6f; t.sz=0.6f; break;
            case 2: t.tx=-1.1f; t.ty=3.0f; t.rx=-legSwing; t.sx=0.5f; t.sy=1.5f; t.sz=0.5f; break; // Pivot moved to 3.0f
            case 3: t.tx=1.1f; t.ty=3.0f; t.rx=legSwing + rightArmGreet; t.sx=0.5f; t.sy=1.5f; t.sz=0.5f; break; // Pivot moved to 3.0f
            case 4: t.tx=-0.4f; t.ty=1.5f; t.rx=legSwing; t.sx=0.6f; t.sy=1.5f; t.sz=0.6f; break; // Pivot moved up to 1.5f
            case 5: t.tx=0.4f; t.ty=1.5f; t.rx=-legSwing; t.sx=0.6f; t.sy=1.5f; t.sz=0.6f; break; // Pivot moved up to 1.5f
            case 6: t.tx=-0.8f; t.ty=2.0f; t.tz=-0.5f; t.sx=0.3f; t.sy=0.3f; t.sz=0.3f; break;
            case 7: t.tx=0.8f; t.ty=2.0f; t.tz=-0.5f; t.sx=0.3f; t.sy=0.3f; t.sz=0.3f; break;
            case 8: t.tx=-0.8f; t.ty=3.0f; t.tz=-0.5f; t.sx=0.3f; t.sy=0.3f; t.sz=0.3f; break;
            case 9: t.tx=0.8f; t.ty=3.0f; t.tz=-0.5f; t.sx=0.3f; t.sy=0.3f; t.sz=0.3f; break;
            case 10: t.ty=2.8f; t.tz=0.45f; t.sx=0.15f; break; // Chest
            case 11: t.ty=2.5f; t.sx=0.0f; break;
            case 12: t.ty=2.5f; t.sx=0.0f; break;
            case 13: t.ty=2.5f; t.sx=0.0f; break;
        }
        break;
    case CAR:
        switch(partIdx) {
            case 0: t.ty=0.6f; t.sx=2.0f; t.sy=0.6f; t.sz=4.0f; break;
            case 1: t.ty=1.1f; t.sx=1.2f; t.sy=0.5f; t.sz=1.5f; break;
            case 2: t.tx=-0.5f; t.ty=0.5f; t.tz=-0.5f; t.sx=0.1f; t.sy=0.1f; t.sz=0.1f; break;
            case 3: t.tx=0.5f; t.ty=0.5f; t.tz=-0.5f; t.sx=0.1f; t.sy=0.1f; t.sz=0.1f; break;
            case 4: t.tx=-0.5f; t.ty=0.5f; t.tz=0.0f; t.sx=0.1f; t.sy=0.1f; t.sz=0.1f; break; // Legs hidden deeper
            case 5: t.tx=0.5f; t.ty=0.5f; t.tz=0.0f; t.sx=0.1f; t.sy=0.1f; t.sz=0.1f; break; // Legs hidden deeper
            case 6: t.tx=-1.1f; t.ty=0.4f; t.tz=1.4f; t.rx=wheelRotation; t.sx=0.4f; t.sy=0.4f; t.sz=0.4f; break;
            case 7: t.tx=1.1f; t.ty=0.4f; t.tz=1.4f; t.rx=wheelRotation; t.sx=0.4f; t.sy=0.4f; t.sz=0.4f; break;
            case 8: t.tx=-1.1f; t.ty=0.4f; t.tz=-1.4f; t.rx=wheelRotation; t.sx=0.4f; t.sy=0.4f; t.sz=0.4f; break;
            case 9: t.tx=1.1f; t.ty=0.4f; t.tz=-1.4f; t.rx=wheelRotation; t.sx=0.4f; t.sy=0.4f; t.sz=0.4f; break;
            case 10: t.ty=0.6f; t.sx=0.0f; break;
            case 11: t.tx=-0.7f; t.ty=0.6f; t.tz=2.05f; t.sx=0.15f; break; // Headlight L
            case 12: t.tx=0.7f; t.ty=0.6f; t.tz=2.05f; t.sx=0.15f; break; // Headlight R
            case 13: t.ty=0.6f; t.sx=0.0f; break;
        }
        break;
    case PLANE:
        switch(partIdx) {
            case 0: t.ty=1.0f; t.sx=1.0f; t.sy=1.0f; t.sz=5.0f; break;
            case 1: t.ty=0.8f; t.tz=0.0f; t.sx=0.1f; t.sy=0.1f; t.sz=0.1f; break; // Head hidden deeper
            case 2: t.tx=-1.5f; t.ty=1.0f; t.sx=2.0f; t.sy=0.1f; t.sz=1.5f; break;
            case 3: t.tx=1.5f; t.ty=1.0f; t.sx=2.0f; t.sy=0.1f; t.sz=1.5f; break;
            case 4: t.tx=-0.8f; t.ty=1.0f; t.tz=-2.0f; t.sx=1.0f; t.sy=0.1f; t.sz=0.8f; break;
            case 5: t.tx=0.8f; t.ty=1.0f; t.tz=-2.0f; t.sx=1.0f; t.sy=0.1f; t.sz=0.8f; break;
            case 6: case 7: case 8: case 9: // Wheels hidden deeper in plane
                t.ty=1.0f; t.tz=0.0f; t.sx=0.01f; t.sy=0.01f; t.sz=0.01f; break;
            case 10: t.ty=1.2f; t.tz=2.5f; t.sx=0.15f; break; // Light at nose
            case 11: case 12: case 13: t.ty=1.0f; t.sx=0.0f; break;
        }
        break;
    case BOAT:
        switch(partIdx) {
            case 0: t.ty=0.5f; t.sx=2.5f; t.sy=0.8f; t.sz=5.0f; break;
            case 1: t.ty=1.3f; t.tz=0.5f; t.sx=1.0f; t.sy=0.8f; t.sz=1.0f; break;
            case 2: t.tx=-0.8f; t.ty=1.0f; t.tz=-1.5f; t.sx=0.4f; t.sy=0.4f; t.sz=1.5f; break;
            case 3: t.tx=0.8f; t.ty=1.0f; t.tz=-1.5f; t.sx=0.4f; t.sy=0.4f; t.sz=1.5f; break;
            case 4: t.tx=-0.8f; t.ty=1.0f; t.tz=1.5f; t.sx=0.4f; t.sy=0.4f; t.sz=1.5f; break;
            case 5: t.tx=0.8f; t.ty=1.0f; t.tz=1.5f; t.sx=0.4f; t.sy=0.4f; t.sz=1.5f; break;
            case 6: t.tx=-1.35f; t.ty=0.6f; t.tz=-1.5f; t.rz=90; t.sx=0.3f; t.sy=0.3f; t.sz=0.3f; break;
            case 7: t.tx=1.35f; t.ty=0.6f; t.tz=-1.5f; t.rz=90; t.sx=0.3f; t.sy=0.3f; t.sz=0.3f; break;
            case 8: t.tx=-1.35f; t.ty=0.6f; t.tz=0.5f; t.rz=90; t.sx=0.3f; t.sy=0.3f; t.sz=0.3f; break;
            case 9: t.tx=1.35f; t.ty=0.6f; t.tz=0.5f; t.rz=90; t.sx=0.3f; t.sy=0.3f; t.sz=0.3f; break;
            case 10: case 11: case 12: t.ty=0.5f; t.sx=0.0f; break;
            case 13: t.ty=2.2f; t.tz=0.5f; t.sx=0.2f; break; // Boat light
        }
        break;
    }
    return t;
}

float lerp(float a, float b, float f) { return a + f * (b - a); }

void Robot::draw() {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotY, 0, 1, 0);
    for (int i = 0; i < 14; i++) drawPart(i, transformFactor);
    glPopMatrix();
}

void Robot::drawPart(int partIdx, float factor) {
    PartTransform t1 = getPartTransform(partIdx, currentForm);
    PartTransform t2 = getPartTransform(partIdx, targetForm);

    float tx = lerp(t1.tx, t2.tx, factor);
    float ty = lerp(t1.ty, t2.ty, factor);
    float tz = lerp(t1.tz, t2.tz, factor);
    float rx = lerp(t1.rx, t2.rx, factor);
    float ry = lerp(t1.ry, t2.ry, factor);
    float rz = lerp(t1.rz, t2.rz, factor);
    float sx = lerp(t1.sx, t2.sx, factor);
    float sy = lerp(t1.sy, t2.sy, factor);
    float sz = lerp(t1.sz, t2.sz, factor);

    if (partIdx == 0) glColor3f(0.8f, 0.1f, 0.1f);
    else if (partIdx == 1) glColor3f(0.9f, 0.8f, 0.2f);
    else if (partIdx >= 2 && partIdx <= 5) glColor3f(0.1f, 0.1f, 0.8f);
    else if (partIdx >= 6 && partIdx <= 9) glColor3f(0.2f, 0.2f, 0.2f);
    else {
        // Lights color with blinking for Greet
        if (greetingTimer > 0 && (targetForm == CAR || targetForm == BOAT)) {
            // Blink twice in 2 seconds
            float blink = sin(greetingTimer * 3.14159f * 2.0f); // 2 full waves in 2s
            if (blink > 0) glColor3f(1.0f, 1.0f, 1.0f); // White
            else glColor3f(0.3f, 0.3f, 0.3f); // Darker grey
        } else {
            glColor3f(0.3f, 0.3f, 0.3f); // Default grey
        }
    }

    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glRotatef(rx, 1, 0, 0);
    glRotatef(ry, 0, 1, 0);
    glRotatef(rz, 0, 0, 1);
    
    // Offset for humanoid limbs to rotate from their top
    if (targetForm == HUMANOID) {
        if (partIdx >= 2 && partIdx <= 3) glTranslatef(0, -0.75f, 0); // Arms
        else if (partIdx >= 4 && partIdx <= 5) glTranslatef(0, -0.75f, 0); // Legs
    }

    if (partIdx >= 10) {
        glPushAttrib(GL_LIGHTING_BIT);
        glDisable(GL_LIGHTING);
        glutSolidSphere(sx, 10, 10);
        glPopAttrib();
    } else if (partIdx >= 6) {
        drawWheel(sx, sy);
    } else {
        drawCube(sx, sy, sz);
    }
    glPopMatrix();
}

