#pragma once

#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "Vector3.hpp"
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

    void addMeshAssets(std::vector<std::string> filenames) {
        for (std::string filename : filenames) {
            addMeshAsset(filename);
        }
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

    void stepForces(double dt) {
        for (const Mapping &m : _map) {
            m.stepVelocity(*this, next[m.id], dt);
        }
    }

    void stepPositions(double dt) {
        for (const Mapping &m : _map) {
            m.stepPosition(*this, next[m.id], dt);
        }
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
                    std::swap(contacts[k].a, contacts[k].b);
                    contacts[k].normal = -contacts[k].normal;
                }
            }
        }
    }

    // https://github.com/erincatto/box2d-lite (Arbiter::{PreStep,ApplyImpulse})
    void resolveContacts(double dt) {
        float dtInv = static_cast<float>(1.0 / dt);

        for (Contact &c : contacts) {
            preSolve(c, dtInv);
        }
        for (int i = 0; i < ITERATIONS; i++) {
            for (Contact &c : contacts) {
                solveContact(c);
            }
        }
    }

    inline void preSolve(Contact &c, float dtInv) {
        const State &si = next[c.i];
        const State &sj = next[c.j];

        c.ra = c.a - si.position;
        c.rb = c.b - sj.position;

        tangentBasis(c.normal, c.t1, c.t2);
        c.massN = effectiveMass(si, sj, c.ra, c.rb, c.normal);
        c.massT1 = effectiveMass(si, sj, c.ra, c.rb, c.t1);
        c.massT2 = effectiveMass(si, sj, c.ra, c.rb, c.t2);

        float positional =
            BIAS_FACTOR * dtInv * std::max(0.0f, c.depth - ALLOWED_PENETRATION);
        float vn = c.normal.DotProduct(relativeVelocity(c));
        float e = (si.restitution + sj.restitution) / 2.0f;
        float bounce = vn < -RESTITUTION_THRESHOLD ? -e * vn : 0.0f;
        c.bias = std::max(positional, bounce);
    }

    inline static void tangentBasis(
        const raylib::Vector3 &n,
        raylib::Vector3 &t1,
        raylib::Vector3 &t2
    ) {
        raylib::Vector3 axis = std::abs(n.x) < THIRD_ROOT_INV
                                   ? raylib::Vector3(1, 0, 0)
                                   : raylib::Vector3(0, 1, 0);
        t1 = n.CrossProduct(axis).Normalize();
        t2 = n.CrossProduct(t1);
    }

    inline static float effectiveMass(
        const State &si,
        const State &sj,
        const raylib::Vector3 &ra,
        const raylib::Vector3 &rb,
        const raylib::Vector3 &d
    ) {
        float ca =
            d.DotProduct((si.momentInv * ra.CrossProduct(d)).CrossProduct(ra));
        float cb =
            d.DotProduct((sj.momentInv * rb.CrossProduct(d)).CrossProduct(rb));
        float k = si.massInv() + sj.massInv() + ca + cb;
        return k > 0.0f ? 1.0f / k : 0.0f;
    }

    inline void solveContact(Contact &c) {
        const State &si = next[c.i];
        const State &sj = next[c.j];

        float vn = c.normal.DotProduct(relativeVelocity(c));
        accumulate(c, c.normal, c.pn, c.massN * (c.bias - vn), 0.0f, INFINITY);

        float maxPt = std::sqrt(si.friction * sj.friction) * c.pn;
        float vt1 = c.t1.DotProduct(relativeVelocity(c));
        accumulate(c, c.t1, c.pt1, -c.massT1 * vt1, -maxPt, maxPt);
        float vt2 = c.t2.DotProduct(relativeVelocity(c));
        accumulate(c, c.t2, c.pt2, -c.massT2 * vt2, -maxPt, maxPt);
    }

    inline void accumulate(
        const Contact &c,
        const raylib::Vector3 &d,
        float &p,
        float dp,
        float lo,
        float hi
    ) {
        float p0 = p;
        p = std::clamp(p0 + dp, lo, hi);
        raylib::Vector3 r = d * (p - p0);

        State &si = next[c.i];
        State &sj = next[c.j];
        if (!si._static) {
            si.velocity += r * si.massInv();
            si.angularMomentum += c.ra.CrossProduct(r);
            si.updateAngular();
        }
        if (!sj._static) {
            sj.velocity -= r * sj.massInv();
            sj.angularMomentum -= c.rb.CrossProduct(r);
            sj.updateAngular();
        }
    }

    inline raylib::Vector3 relativeVelocity(const Contact &c) const {
        const State &si = next[c.i];
        const State &sj = next[c.j];
        raylib::Vector3 va = si.velocity + si.angular.CrossProduct(c.ra);
        raylib::Vector3 vb = sj.velocity + sj.angular.CrossProduct(c.rb);
        return va - vb;
    }
};
