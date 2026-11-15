#pragma once

// #include "Quaternion.hpp"
#include "Vector3.hpp"

#include "structures/Mat3.hpp"

struct State {
    State()
        : _static(false), _active(true), scale(1.0f), friction(0.0f),
          restitution(1.0f), mass(1.0f), position(raylib::Vector3::Zero()),
          velocity(raylib::Vector3::Zero()), force(raylib::Vector3::Zero()),
          moment(Mat3::identity()), orientation(Mat3::identity()),
          angular(raylib::Vector3::Zero()), torque(raylib::Vector3::Zero()) {
    }

    bool _static;
    bool _active;

    float scale;
    float friction;
    float restitution;

    // --- integrated ---

    float mass;
    raylib::Vector3 position;
    raylib::Vector3 velocity;
    raylib::Vector3 force;

    Mat3 moment;
    Mat3 orientation;
    raylib::Vector3 angular;
    raylib::Vector3 torque;
};