#pragma once

#include <vector>

#include "structures/State.hpp"

// forward reference
struct Arena;

struct Mapping {
  private:
    void updateMoment(const Arena &arena, State &next) const;

  public:
    int id;
    int asset;
    std::vector<int> forces;

    // forces -> velocity (copies entities[id] into next first)
    void stepVelocity(const Arena &arena, State &next, double dt) const;
    // velocity -> position/orientation, run after contacts are resolved
    void stepPosition(const Arena &arena, State &next, double dt) const;
};