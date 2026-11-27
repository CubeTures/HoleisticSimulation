#pragma once

#include <Vector3.hpp>

#include "structures/State.hpp"

// forward reference
struct Arena;

class FlockForce {
  public:
    raylib::Vector3 at(const Arena &arena, const State &state) const {
        // TODO implement in a cpp
        return raylib::Vector3::Zero();
    }
};