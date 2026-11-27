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
	state.restitution = 0.1;
    state.position = raylib::Vector3(0, 2, 0);
    state.orientation = Mat3::fromQuaternion(QuaternionFromAxisAngle(
        raylib::Vector3(1, 0, 1).Normalize(), 30 * DEG2RAD
    ));
    arena->addEntity(state, MeshCollider{}, MeshGraphics{}, {gravity}, cube);

    state = State();
	state._static = true;
	state.restitution = 0.1;
    state.scale = 5;
    state.mass = 1;
    state.position = raylib::Vector3(1, -5, 1);
    arena->addEntity(state, MeshCollider{}, MeshGraphics{}, {}, cube);

    return arena;
}