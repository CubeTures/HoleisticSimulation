#pragma once

#include <variant>

#include "generators/ParticleGenerator.hpp"

using Generator = std::variant<ParticleGenerator>;