#pragma once

#include <vector>

#include "Vector3.hpp"

#include "structures/TriangleIndex.hpp"

struct MeshView {
    const std::vector<raylib::Vector3> &positions;
    const std::vector<TriangleIndex> &triangles;
};
