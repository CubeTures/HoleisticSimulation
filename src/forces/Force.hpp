#pragma once

#include <variant>

#include "forces/FlockForce.hpp"
#include "forces/FunctionalForce.hpp"

using Force = std::variant<FunctionalForce, FlockForce>;