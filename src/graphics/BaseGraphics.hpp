#pragma once

#include <variant>

#include "shaders/BlinnPhongShader.hpp"
#include "shaders/ParticleShader.hpp"

struct BaseGraphics {
    using Material =
        std::variant<BlinnPhongShader::Material, ParticleShader::Material>;

    Material material;
};
