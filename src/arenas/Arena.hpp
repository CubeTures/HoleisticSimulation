#pragma once

#include <unordered_map>
#include <vector>

#include <Camera3D.hpp>
#include <Functions.hpp>
#include <Window.hpp>

#include "assets/MeshAsset.hpp"
#include "colliders/Collider.hpp"
#include "forces/Force.hpp"
#include "generators/Generator.hpp"
#include "graphics/Graphics.hpp"
#include "structures/Mapping.hpp"
#include "structures/State.hpp"

struct Arena {
    std::vector<Mapping> _map;
    std::unordered_map<std::string, MeshAsset> assets;

    std::vector<State> entities;
    std::vector<Collider> colliders;
    std::vector<Graphics> graphics;

    std::vector<Force> forces;
    std::vector<Generator> generators;

    void addMeshAsset(std::string filename) {
        assets.insert({filename, MeshAsset(filename)});
    }

    void addEntity(
        State state,
        Collider collider,
        Graphics graphic,
        std::string asset
    ) {
        entities.push_back(state);
        colliders.push_back(collider);
        graphics.push_back(graphic);
        _map.push_back(Mapping{
            asset,
            static_cast<int>(entities.size()) - 1,
            static_cast<int>(colliders.size()) - 1,
            static_cast<int>(graphics.size()) - 1
        });
    }
};
