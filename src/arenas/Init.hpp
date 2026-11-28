#pragma once

#include "Color.hpp"
#include "Vector3.hpp"
#include <Camera3D.hpp>
#include <Functions.hpp>
#include <Window.hpp>

#include "arenas/Arena.hpp"
#include "colliders/Collider.hpp"
#include "forces/FunctionalForce.hpp"
#include "graphics/MeshGraphics.hpp"
#include "shaders/BlinnPhongShader.hpp"
#include "structures/State.hpp"

Arena *init() {
    Arena *arena = new Arena();

    std::string cube = "/models/obj/cube.obj";
    std::string ball = "/models/obj/ball.obj";
    arena->addMeshAssets({cube, ball});

    std::string gravity = "gravity";
    arena->addForce(gravity, FunctionalForce([](const State &s) {
                        return raylib::Vector3(0, -9.8, 0) * s.mass;
                    }));

    State state;
    state.restitution = 0.1;
    state.position = raylib::Vector3(-1, 2, 0);
    state.orientation = Mat3::fromQuaternion(QuaternionFromAxisAngle(
        raylib::Vector3(1, 0, 1).Normalize(), 30 * DEG2RAD
    ));
    arena->addEntity(
        state,
        MeshCollider{},
        MeshGraphics{
            BlinnPhongShader::Material::fromColor(raylib::Color::Blue()),
        },
        {gravity},
        cube
    );

    state = State();
    state.scale = 0.5;
    state.restitution = 0.1;
    state.position = raylib::Vector3(1, 2, 0);
    arena->addEntity(
        state,
        SphereCollider{},
        MeshGraphics{
            BlinnPhongShader::Material::fromColor(raylib::Color::Blue()),
        },
        {},
        ball
    );

    state = State();
    state._static = true;
    state.restitution = 0.1;
    state.scale = 5;
    state.mass = 1;
    state.position = raylib::Vector3(0, -5, 0);
    arena->addEntity(
        state,
        MeshCollider{},
        MeshGraphics{
            BlinnPhongShader::Material::fromColor(raylib::Color::Red()),
        },
        {},
        cube
    );

    return arena;
}