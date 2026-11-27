#pragma once

#include <functional>

#include <Vector3.hpp>

#include "structures/State.hpp"

class FunctionalForce {
  private:
    using Fn = std::function<const raylib::Vector3(const State &)>;
    Fn fn;

  public:
    FunctionalForce() : FunctionalForce(raylib::Vector3::Zero()) {
    }

    FunctionalForce(raylib::Vector3 direction)
        : FunctionalForce([direction](const State &) -> raylib::Vector3 {
              return direction;
          }) {
    }

    FunctionalForce(Fn fn) : fn(fn) {
    }

    raylib::Vector3 at(const State &state) const {
        return fn(state);
    }
};