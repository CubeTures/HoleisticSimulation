#pragma once

#include "Vector3.hpp"

struct Contact {
    int i;
    int j;

    raylib::Vector3 point;
    raylib::Vector3 normal;

    float depth;
};
