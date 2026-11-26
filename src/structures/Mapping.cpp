#include "structures/Mapping.hpp"

#include "arenas/Arena.hpp"
#include "graphics/Graphics.hpp"
#include "structures/Overloaded.hpp"
#include "structures/State.hpp"

void Mapping::draw(Arena *arena) {
    MeshAsset &a = arena->assets.at(asset);
    State &s = arena->entities[entity];
    Graphics &g = arena->graphics[graphic];

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
            [&](const MeshGraphics &g) { g.draw(s, a); },
        },
        g
    );
}