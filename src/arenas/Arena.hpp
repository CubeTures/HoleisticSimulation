#pragma once

#include <unordered_map>
#include <variant>
#include <vector>

#include <Camera3D.hpp>
#include <Functions.hpp>
#include <Window.hpp>

#include "assets/MeshAsset.hpp"
#include "colliders/Collider.hpp"
#include "colliders/MeshMesh.hpp"
#include "colliders/MeshPlane.hpp"
#include "colliders/SphereMesh.hpp"
#include "colliders/SpherePlane.hpp"
#include "colliders/SphereSphere.hpp"
#include "forces/Force.hpp"
#include "generators/Generator.hpp"
#include "graphics/Graphics.hpp"
#include "structures/Contact.hpp"
#include "structures/Mapping.hpp"
#include "structures/Overloaded.hpp"
#include "structures/State.hpp"

struct Arena {
  private:
    const MeshAsset &asset(int i) const {
        return assets.at(_map[i].asset);
    }

  public:
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

    using Candidate = std::pair<int, int>;
    std::vector<Candidate> candidates;
    std::vector<Contact> contacts;

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

    void updateCandidates() {
        candidates.clear();
        int n = static_cast<int>(next.size());

        for (int i = 0; i < n; i++) {
            const State &si = next[i];

            if (!si._active || si._static) {
                continue;
            }

            for (int j = 0; j < n; j++) {
                const State &sj = next[j];

                if (i == j || !sj._active) {
                    continue;
                } else if (i > j && !sj._static) {
                    continue;
                }

                candidates.push_back({i, j});
            }
        }
    }

    void updateContacts() {
        contacts.clear();

        for (auto [i, j] : candidates) {
            const size_t prev = contacts.size();
            const State &si = next[i];
            const State &sj = next[j];

            bool flipped = std::visit(
                overloaded{
                    [&](const SphereCollider &, const SphereCollider &) {
                        sphereSphere(si, sj, contacts);
                        return false;
                    },

                    [&](const SphereCollider &, const PlaneCollider &) {
                        spherePlane(si, sj, contacts);
                        return false;
                    },
                    [&](const PlaneCollider &, const SphereCollider &) {
                        spherePlane(sj, si, contacts);
                        return true;
                    },

                    [&](const SphereCollider &, const MeshCollider &) {
                        sphereMesh(si, sj, asset(j), contacts);
                        return false;
                    },
                    [&](const MeshCollider &, const SphereCollider &) {
                        sphereMesh(sj, si, asset(i), contacts);
                        return true;
                    },

                    [&](const MeshCollider &, const PlaneCollider &) {
                        meshPlane(si, asset(i), sj, contacts);
                        return false;
                    },
                    [&](const PlaneCollider &, const MeshCollider &) {
                        meshPlane(sj, asset(j), si, contacts);
                        return true;
                    },

                    [&](const MeshCollider &, const MeshCollider &) {
                        meshMesh(si, asset(i), sj, asset(j), contacts);
                        return false;
                    },

                    [](const auto &, const auto &) { return false; },
                },
                colliders[i],
                colliders[j]
            );

            for (size_t k = prev; k < contacts.size(); k++) {
                contacts[k].i = i;
                contacts[k].j = j;
                if (flipped) {
                    contacts[k].normal = -contacts[k].normal;
                }
            }
        }
    }
};
