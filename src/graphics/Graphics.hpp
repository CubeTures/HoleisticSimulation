#pragma once

#include <variant>

#include "graphics/MeshGraphics.hpp"
#include "graphics/NoneGraphics.hpp"
#include "graphics/PlaneGraphics.hpp"
#include "graphics/SphereGraphics.hpp"

// forward reference
struct State;

using Graphics =
    std::variant<NoneGraphics, SphereGraphics, PlaneGraphics, MeshGraphics>;

