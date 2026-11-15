#include <vector>

#include "generators/Generator.hpp"
#include "graphics/Graphics.hpp"

#include "assets/Asset.hpp"
#include "colliders/Collider.hpp"
#include "forces/Force.hpp"
#include "structures/State.hpp"

struct Arena {
    std::vector<Asset> assets;
    std::vector<Collider> colliders;
    std::vector<State> entities;
    std::vector<Force> forces;
    std::vector<Generator> generators;
    std::vector<Graphics> graphics;
};

int main() {
    Arena arena;

    // need some system to create associations

    return 0;
}

/*
    Need to handle:
    forces (gravity, wind)
    rigid bodies
    particles, flocks
        - become generators allocate
    shaders
    input
    camera
    heightmap -> mesh
    bvh
    collisions
        -
    integration methods
        -
*/