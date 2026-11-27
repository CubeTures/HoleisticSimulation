#pragma once

#include "Vector3.hpp"

struct Contact {
    int i;
    int j;

    raylib::Vector3 a, b;
    raylib::Vector3 normal;

    float depth;

    Contact(
        raylib::Vector3 a,
        raylib::Vector3 b,
        raylib::Vector3 normal,
        float depth
    )
        : a(a), b(b), normal(normal), depth(depth) {
    }
};
