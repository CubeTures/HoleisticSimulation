#pragma once

#include <Color.hpp>

#include "assets/MeshAsset.hpp"
#include "graphics/BaseGraphics.hpp"
#include "structures/State.hpp"

class MeshGraphics : public BaseGraphics {
  public:
    void draw(const State &state, const MeshAsset &asset) const {
        ::Model m = asset.model;
        m.transform = state.orientation.toMatrix();
        ::DrawModel(m, state.position, state.scale, raylib::Color::White());
    }
};
