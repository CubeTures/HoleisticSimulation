#pragma once

#include <Camera3D.hpp>
#include <Functions.hpp>
#include <Window.hpp>

#include "arenas/Arena.hpp"
#include "colliders/MeshCollider.hpp"
#include "graphics/MeshGraphics.hpp"
#include "structures/State.hpp"

Arena *init() {
    Arena *arena = new Arena();

    std::string cube = "/models/obj/cube.obj";
    arena->addMeshAsset(cube);

    State state;
    arena->addEntity(state, MeshCollider{}, MeshGraphics{}, cube);

    return arena;
}