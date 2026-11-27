#pragma once

#include "Vector3.hpp"
#include <Camera3D.hpp>
#include <Functions.hpp>
#include <Window.hpp>

#include "arenas/Arena.hpp"
#include "colliders/Collider.hpp"
#include "forces/FunctionalForce.hpp"
#include "graphics/MeshGraphics.hpp"
#include "structures/State.hpp"

Arena *init() {
    Arena *arena = new Arena();

    std::string cube = "/models/obj/cube.obj";
    arena->addMeshAsset(cube);

    std::string gravity = "gravity";
    arena->addForce(gravity, FunctionalForce([](const State &s) {
                        return raylib::Vector3(0, -9.8, 0) * s.mass;
                    }));

    State state;
    state.position = raylib::Vector3(0, 2, 0);
    arena->addEntity(state, MeshCollider{}, MeshGraphics{}, {gravity}, cube);

    state = State();
    state.scale = 2;
    state.position = raylib::Vector3(1, -1, 1);
    arena->addEntity(state, MeshCollider{}, MeshGraphics{}, {}, cube);

    return arena;
}