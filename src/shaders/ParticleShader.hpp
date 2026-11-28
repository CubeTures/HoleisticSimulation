#pragma once

#include <Color.hpp>
#include <Shader.hpp>

#include "structures/DrawContext.hpp"

class ParticleShader {
  public:
    struct Material {
        raylib::Color color;
    };

    raylib::Shader shader;

    ParticleShader() : shader(nullptr, RES_DIR "/shaders/particle_frag.glsl") {
    }

    void beginMode(const DrawContext &) const {
        ::BeginShaderMode(shader);
    }

    void draw(const DrawContext &ctx) const;

    void endMode() const {
        ::EndShaderMode();
    }
};
