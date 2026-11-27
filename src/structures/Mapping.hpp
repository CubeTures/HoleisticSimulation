#pragma once

#include <vector>

#include "structures/State.hpp"

// forward reference
struct Arena;

struct Mapping {
    int id;
    int asset;
    std::vector<int> forces;

    void step(const Arena &arena, State &next, double dt) const;
    void collide(const Arena &arena, State &next) const;
    void draw(const Arena &arena) const;
};