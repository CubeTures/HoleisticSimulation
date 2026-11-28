#pragma once

#include <concepts>

#include "shaders/BlinnPhongShader.hpp"
#include "shaders/ParticleShader.hpp"
#include "structures/DrawContext.hpp"

template <typename T>
concept IShader =
    std::movable<T> && requires(const T &t, const DrawContext &ctx) {
        { t.beginMode(ctx) } -> std::same_as<void>;
        { t.draw(ctx) } -> std::same_as<void>;
        { t.endMode() } -> std::same_as<void>;
    };

static_assert(IShader<BlinnPhongShader>);
static_assert(IShader<ParticleShader>);
