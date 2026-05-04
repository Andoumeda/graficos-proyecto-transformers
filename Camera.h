#ifndef CAMERA_H
#define CAMERA_H

#include <GL/freeglut.h>
#include <cmath>

class Camera {
public:
    float distance;
    float azimuth;
    float elevation;
    float centerX, centerY, centerZ;

    Camera() : distance(15.0f), azimuth(45.0f), elevation(30.0f), centerX(0), centerY(0), centerZ(0) {}

    // Aplica la vista de la camara al framebuffer.
    void apply() {
        float x = distance * cos(elevation * 3.14159f / 180.0f) * sin(azimuth * 3.14159f / 180.0f);
        float y = distance * sin(elevation * 3.14159f / 180.0f);
        float z = distance * cos(elevation * 3.14159f / 180.0f) * cos(azimuth * 3.14159f / 180.0f);

        gluLookAt(x + centerX, y + centerY, z + centerZ,
            centerX, centerY, centerZ,
            0, 1, 0);
    }

    // Aumenta o reduce la distancia al centro de la escena.
    void zoom(float delta) {
        distance += delta;
        if (distance < 2.0f) distance = 2.0f;
        if (distance > 50.0f) distance = 50.0f;
    }

    // Rota la camara con los deltas de azimuth y elevacion.
    void rotate(float dAzimuth, float dElevation) {
        azimuth += dAzimuth;
        elevation += dElevation;
        if (elevation > 89.0f) elevation = 89.0f;
        if (elevation < -89.0f) elevation = -89.0f;
    }
};

#endif
