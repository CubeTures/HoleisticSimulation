#include "structures/Mapping.hpp"

#include "arenas/Arena.hpp"
#include "colliders/Collider.hpp"
#include "forces/FlockForce.hpp"
#include "forces/FunctionalForce.hpp"
#include "graphics/Graphics.hpp"
#include "structures/Overloaded.hpp"
#include "structures/State.hpp"

void Mapping::step(const Arena &arena, State &next, double dt) const {
    next = arena.entities[id];

    if (!next._active || next._static) {
        return;
    }

    raylib::Vector3 f = raylib::Vector3::Zero();
    for (int i : forces) {
        f += std::visit(
            overloaded{
                [&](const FunctionalForce &f) { return f.at(next); },
                [&](const FlockForce &f) { return f.at(arena, next); },
            },
            arena.forces[i]
        );
    }

    raylib::Vector3 acc = f / next.mass;
    next.velocity = next.velocity + acc * dt;
    next.position = next.position + next.velocity * dt;
}

void Mapping::collide(const Arena &arena, State &next) const {
    const Collider &collider = arena.colliders[id];

    if (!next._active || next._static) {
        return;
    }

    std::visit(
        overloaded{
            [&](const NoneCollider &) {
                // no-op
            },
            [&](const SphereCollider &c) {
                // TODO
				// all
            },
            [&](const PlaneCollider &c) {
                // TODO
				// sphere, mesh
            },
            [&](const MeshCollider &c) {
                // TODO
				// all
            },
        },
        collider
    );
}

void Mapping::draw(const Arena &arena) const {
    const Graphics &graphic = arena.graphics[id];
    const State &state = arena.entities[id];

    std::visit(
        overloaded{
            [](const NoneGraphics &) {
                // no-op
            },
            [&](const SphereGraphics &g) {
                // TODO
            },
            [&](const PlaneGraphics &g) {
                // TODO
            },
            [&](const MeshGraphics &g) {
                const MeshAsset &a = arena.assets.at(asset);
                g.draw(state, a);
            },
        },
        graphic
    );
}