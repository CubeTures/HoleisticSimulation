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

    void step(const Arena &arena, State &next, double dt) const;
};