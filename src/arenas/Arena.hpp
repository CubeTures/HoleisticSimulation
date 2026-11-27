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
    std::vector<MeshAsset> assets;
    std::unordered_map<std::string, int> assetsMap;

    std::vector<State> entities;
    std::vector<State> next;

    std::vector<Collider> colliders;
    std::vector<Graphics> graphics;

    std::vector<Force> forces;
    std::unordered_map<std::string, int> forcesMap;
    std::vector<Generator> generators;

    void addMeshAsset(std::string filename) {
        int index = static_cast<int>(assets.size());
        assets.push_back(MeshAsset(filename));
        assetsMap.insert({filename, index});
    }

    void addForce(std::string name, Force force) {
        int index = static_cast<int>(forces.size());
        forces.push_back(std::move(force));
        forcesMap.emplace(name, index);
    }

    void addEntity(
        State state,
        Collider collider,
        Graphics graphic,
        std::vector<std::string> forces,
        std::string asset = "__none__"
    ) {
        entities.push_back(state);
        next.push_back(state);
        colliders.push_back(collider);
        graphics.push_back(graphic);

        int assetId = -1;
        if (asset != "__none__") {
            assetId = assetsMap.at(asset);
        }

        std::vector<int> forceIds;
        for (std::string f : forces) {
            forceIds.push_back(forcesMap.at(f));
        }

        _map.push_back(Mapping{static_cast<int>(_map.size()), assetId, forceIds}
        );
    }
};
