#pragma once

#include <vector>

#include "Constants.hpp"

#include "assets/MeshAsset.hpp"
#include "structures/Contact.hpp"
#include "structures/State.hpp"

inline void meshMesh(
    const State &si,
    const MeshAsset &mi,
    const State &sj,
    const MeshAsset &mj,
    std::vector<Contact> &contacts
) {
    std::unordered_map<int, std::vector<int>> vertices;
    mi.walk(si, sj, mj, vertices);

    for (const auto &[i, candidates] : vertices) {
        raylib::Vector3 world =
            si.orientation * mi.positions[i] * si.scale + si.position;
        raylib::Vector3 pos =
            sj.orientation.transpose() * (world - sj.position) / sj.scale;

        int closest = candidates[0];
        raylib::Vector3 pointClosest = mj.triangle(closest).closestPoint(pos);
        float distClosest = pointClosest.Distance(pos);
        for (int index : candidates) {
            raylib::Vector3 point = mj.triangle(index).closestPoint(pos);
            float dist = point.Distance(pos);
            if (dist < distClosest) {
                closest = index;
                pointClosest = point;
                distClosest = dist;
            }
        }

        float distWorld = distClosest * sj.scale;
        if (distWorld > MAX_COLLISION_DIST) {
            continue;
        }

        // NOTE used to be normalAt()
        raylib::Vector3 n = mj.triangle(closest).normal();
        if ((pos - pointClosest).DotProduct(n) > 0) {
            continue;
        }

        n = sj.orientation * n;
        raylib::Vector3 worldOther =
            sj.orientation * (pointClosest * sj.scale) + sj.position;

        contacts.emplace_back(world, worldOther, n, distWorld);
    }
}
