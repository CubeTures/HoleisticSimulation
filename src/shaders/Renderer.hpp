#pragma once

#include <tuple>
#include <utility>

#include <Camera3D.hpp>
#include <Color.hpp>
#include <Vector3.hpp>

#include "shaders/Shader.hpp"
#include "structures/DrawContext.hpp"

template <IShader... Ts> class Renderer {
  private:
    // draw order implied by argument order
    std::tuple<Ts...> shaders;

  public:
    explicit Renderer(Ts... shaders) : shaders(std::move(shaders)...) {
    }

    void draw(const Arena &arena, const raylib::Camera3D &camera) const {
        DrawContext ctx{arena, camera};
        std::apply(
            [&](const auto &...s) {
                ((s.beginMode(ctx), s.draw(ctx), s.endMode()), ...);
            },
            shaders
        );
    }
};
