#include "shaders/ParticleShader.hpp"

#include <variant>

#include <Camera3D.hpp>
#include <Matrix.hpp>
#include <Vector3.hpp>

#include "arenas/Arena.hpp"
#include "graphics/SphereGraphics.hpp"

void ParticleShader::draw(const DrawContext &ctx) const {
    const Arena &arena = ctx.arena;

    const raylib::Camera3D &camera = ctx.camera;
    raylib::Matrix view =
        raylib::Matrix::LookAt(camera.position, camera.target, camera.up);
    raylib::Vector3 up = {view.m1, view.m5, view.m9};

    for (const Mapping &m : arena._map) {
        const auto *g = std::get_if<SphereGraphics>(&arena.graphics[m.id]);
        if (!g) {
            continue;
        }

        const auto *mat = std::get_if<Material>(&g->material);
        if (!mat) {
            continue;
        }

        g->drawBillboard(arena.entities[m.id], camera, up, mat->color);
    }
}
