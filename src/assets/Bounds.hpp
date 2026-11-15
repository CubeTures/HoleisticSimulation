#pragma once

#include <cmath>
#include <limits>

#include "Vector3.hpp"

#include "assets/AABB.hpp"
#include "structures/Mat3.hpp"
#include "structures/MeshView.hpp"
#include "structures/State.hpp"

struct Bounds {

  private:
    raylib::Vector3 pMin = raylib::Vector3(
                        std::numeric_limits<float>::max(),
                        std::numeric_limits<float>::max(),
                        std::numeric_limits<float>::max()
                    ),
                    pMax = raylib::Vector3(
                        std::numeric_limits<float>::lowest(),
                        std::numeric_limits<float>::lowest(),
                        std::numeric_limits<float>::lowest()
                    );

    // https://madmann91.github.io/2024/02/10/converting-oriented-bounding-boxes-to-axis-aligned-ones.html
    AABB convert(const State &state) const {
        raylib::Vector3 center = (pMin + pMax) * 0.5f;
        raylib::Vector3 half = (pMax - pMin) * 0.5f;
        const Mat3 &r = state.orientation;
        float s = state.scale;

        raylib::Vector3 c = world(state, center);
        raylib::Vector3 h(
            s * (std::abs(r.m[0][0]) * half.x + std::abs(r.m[0][1]) * half.y +
                 std::abs(r.m[0][2]) * half.z),
            s * (std::abs(r.m[1][0]) * half.x + std::abs(r.m[1][1]) * half.y +
                 std::abs(r.m[1][2]) * half.z),
            s * (std::abs(r.m[2][0]) * half.x + std::abs(r.m[2][1]) * half.y +
                 std::abs(r.m[2][2]) * half.z)
        );
        return AABB({c - h, c + h});
    }

    raylib::Vector3 world(const State &state, raylib::Vector3 p) const {
        return state.orientation * p * state.scale + state.position;
    }

  public:
    bool overlap(
        const State &state,
        const State &other,
        const Bounds &boundsOther
    ) const {
        return convert(state).overlaps(boundsOther.convert(other));
    }

    float volume() const {
        return (pMax.x - pMin.x) * (pMax.y - pMin.y) * (pMax.z - pMin.z);
    }

    virtual void grow(const MeshView &mesh, const TriangleIndex &tri) {
        grow(mesh.positions[tri.i]);
        grow(mesh.positions[tri.j]);
        grow(mesh.positions[tri.k]);
    }

    void grow(raylib::Vector3 p) {
        if (p.x < pMin.x) {
            pMin.x = p.x;
        }
        if (p.y < pMin.y) {
            pMin.y = p.y;
        }
        if (p.z < pMin.z) {
            pMin.z = p.z;
        }
        if (p.x > pMax.x) {
            pMax.x = p.x;
        }
        if (p.y > pMax.y) {
            pMax.y = p.y;
        }
        if (p.z > pMax.z) {
            pMax.z = p.z;
        }
    }
};