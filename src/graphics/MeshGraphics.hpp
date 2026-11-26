#pragma once

#include <Color.hpp>

#include "assets/MeshAsset.hpp"
#include "structures/State.hpp"

class MeshGraphics {
  public:
    void draw(const State &state, const MeshAsset &asset) const {
        asset.model.Draw(state.position, state.scale, raylib::Color::White());
    }
};