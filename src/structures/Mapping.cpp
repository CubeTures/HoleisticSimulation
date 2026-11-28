#include "structures/Mapping.hpp"

#include <cmath>

#include "arenas/Arena.hpp"
#include "assets/MeshAsset.hpp"
#include "colliders/Collider.hpp"
#include "forces/FlockForce.hpp"
#include "forces/FunctionalForce.hpp"
#include "structures/Overloaded.hpp"
#include "structures/State.hpp"

void Mapping::stepVelocity(const Arena &arena, State &next, double dt) const {
    next = arena.entities[id];

    if (!next._active) {
        return;
    }

    raylib::Vector3 force = raylib::Vector3::Zero();
    raylib::Vector3 torque = raylib::Vector3::Zero();
    for (int i : forces) {
        force += std::visit(
            overloaded{
                [&](const FunctionalForce &f) { return f.at(next); },
                [&](const FlockForce &f) { return f.at(arena, next); },
            },
            arena.forces[i]
        );
    }

    float h = static_cast<float>(dt);

    raylib::Vector3 a = force / next.mass;
    next.velocity = next.velocity + a * h;

    next.angularMomentum = next.angularMomentum + torque * h;
    updateMoment(arena, next);
    next.updateAngular();
}

void Mapping::stepPosition(const Arena &arena, State &next, double dt) const {
    if (!next._active) {
        return;
    }

    float h = static_cast<float>(dt);

    next.position = next.position + next.velocity * h;

    Mat3 rd = Mat3::skew(next.angular) * next.orientation;
    next.orientation = (next.orientation + rd * h).orthonormalize();
    updateMoment(arena, next);
    next.updateAngular();
}

void Mapping::updateMoment(const Arena &arena, State &next) const {
    const Collider &collider = arena.colliders[id];

    if (next._static) {
        next.momentInv = Mat3::zero();
        return;
    }

    std::visit(
        overloaded{
            [&](const SphereCollider &) {
                // TODO
            },
            [&](const MeshCollider &) {
                const MeshAsset &a = arena.assets.at(asset);
                float k =
                    next.mass / a.positions.size() * next.scale * next.scale;
                Mat3 rt = next.orientation.transpose();
                next.moment = next.orientation * a.unitMoment * rt * k;
                next.momentInv =
                    next.orientation * a.unitMomentInv * rt * (1.0f / k);
            },
            [](const auto &) {}
        },
        collider
    );
}
