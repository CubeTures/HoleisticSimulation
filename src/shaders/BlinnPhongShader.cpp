#include "shaders/BlinnPhongShader.hpp"

#include <variant>

#include "arenas/Arena.hpp"
#include "graphics/MeshGraphics.hpp"

void BlinnPhongShader::draw(const DrawContext &ctx) const {
    const Arena &arena = ctx.arena;

    for (const Mapping &m : arena._map) {
        const auto *g = std::get_if<MeshGraphics>(&arena.graphics[m.id]);
        if (!g) {
            continue;
        }

        const auto *mat = std::get_if<Material>(&g->material);
        if (!mat) {
            continue;
        }

        const MeshAsset &asset = arena.assets.at(m.asset);
        setMaterial(*mat);
        bind(asset.model);
        g->draw(arena.entities[m.id], asset);
    }
}
