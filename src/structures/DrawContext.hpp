#pragma once

#include <Camera3D.hpp>

// forward reference
struct Arena;

struct DrawContext {
    const Arena &arena;
    const raylib::Camera3D &camera;
};