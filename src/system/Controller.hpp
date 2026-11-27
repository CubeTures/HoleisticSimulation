#pragma once

#include "Camera3D.hpp"
#include <raylib.h>
#include <rcamera.h>

class Controller {
  private:
    raylib::Camera3D &camera;
    double speed;

  public:
    Controller(raylib::Camera3D &camera, double speed)
        : camera(camera), speed(speed) {
        DisableCursor();
    }

    void listen(double dt) {
        camera.Update(CAMERA_THIRD_PERSON);

        double dist = speed * dt;
        if (IsKeyDown(KEY_SPACE)) {
            CameraMoveUp(&camera, dist);
        }
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            CameraMoveUp(&camera, -dist);
        }
    }
};