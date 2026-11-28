#pragma once

#include <Camera3D.hpp>
#include <Color.hpp>
#include <raymath.h>
#include <rlgl.h>

#include "graphics/BaseGraphics.hpp"
#include "structures/State.hpp"

class SphereGraphics : public BaseGraphics {
  public:
    static constexpr float QUAD_PER_RADIUS = 32.0f / 7.5f;

    void drawBillboard(
        const State &state,
        const ::Camera3D &camera,
        const ::Vector3 &up,
        const ::Color &tint
    ) const {
        ::Texture2D white{
            rlGetTextureIdDefault(), 1, 1, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
        };

        float size = state.scale * QUAD_PER_RADIUS;
        ::DrawBillboardPro(
            camera,
            white,
            {0, 0, 1, 1},
            state.position,
            up,
            {size, size},
            {size / 2, size / 2},
            0.0f,
            tint
        );
    }
};
