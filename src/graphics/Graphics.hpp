#pragma once

#include <variant>

#include "graphics/MeshGraphics.hpp"
#include "graphics/PlaneGraphics.hpp"
#include "graphics/SphereGraphics.hpp"

using Graphics = std::variant<SphereGraphics, PlaneGraphics, MeshGraphics>;