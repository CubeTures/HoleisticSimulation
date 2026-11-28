#pragma once

#include "Vector3.hpp"

struct Contact {
    int i;
    int j;

    raylib::Vector3 a, b;
    raylib::Vector3 normal;

    float depth;

    // --- solver ---

    raylib::Vector3 ra, rb;
    raylib::Vector3 t1, t2;

    float massN = 0.0f, massT1 = 0.0f, massT2 = 0.0f;
    float bias = 0.0f;
    float pn = 0.0f, pt1 = 0.0f, pt2 = 0.0f; // accumulated impulses

    Contact(
        raylib::Vector3 a,
        raylib::Vector3 b,
        raylib::Vector3 normal,
        float depth
    )
        : a(a), b(b), normal(normal), depth(depth) {
    }
};
