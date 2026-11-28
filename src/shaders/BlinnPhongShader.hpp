#pragma once

#include <array>
#include <string>

#include <Color.hpp>
#include <Model.hpp>
#include <Shader.hpp>
#include <Vector3.hpp>

#include "structures/DrawContext.hpp"

class BlinnPhongShader {
  private:
    static constexpr int MAX_LIGHTS = 4;

    enum LightType {
        LIGHT_DIRECTIONAL = 0,
        LIGHT_POINT = 1,
    };

    struct Light {
        bool enabled = false;
        LightType type = LIGHT_DIRECTIONAL;
        raylib::Vector3 position = raylib::Vector3::Zero();
        raylib::Vector3 target = raylib::Vector3::Zero();
        raylib::Color color = raylib::Color::White();
    };

    struct LightLocs {
        int enabled, type, position, target, color;
    };

    std::array<LightLocs, MAX_LIGHTS> lightLocs;
    int viewPosLoc, kaLoc, kdLoc, ksLoc, sLoc;

  public:
    struct Material {
        raylib::Vector3 ka, kd, ks;
        float s;

        static Material fromColor(
            const raylib::Color color,
            float ambient = 0.15f,
            float specular = 0.4f,
            float shininess = 32.0f
        ) {
            raylib::Vector3 c(
                color.r / 255.0f, color.g / 255.0f, color.b / 255.0f
            );
            return Material{
                c * ambient,
                c * (1.0f - ambient),
                raylib::Vector3(specular, specular, specular),
                shininess,
            };
        }
    };

    raylib::Shader shader;
    std::array<Light, MAX_LIGHTS> lights;

    BlinnPhongShader()
        : shader(
              RES_DIR "/shaders/blinn_phong_vert.glsl",
              RES_DIR "/shaders/blinn_phong_frag.glsl"
          ) {
        viewPosLoc = ::GetShaderLocation(shader, "viewPos");
        kaLoc = ::GetShaderLocation(shader, "ka");
        kdLoc = ::GetShaderLocation(shader, "kd");
        ksLoc = ::GetShaderLocation(shader, "ks");
        sLoc = ::GetShaderLocation(shader, "s");

        lights[0] = Light{
            true,
            LIGHT_DIRECTIONAL,
            raylib::Vector3(1, 3, 2),
            raylib::Vector3::Zero(),
            raylib::Color::White(),
        };

        for (int i = 0; i < MAX_LIGHTS; i++) {
            std::string p = "lights[" + std::to_string(i) + "].";
            lightLocs[i] = LightLocs{
                ::GetShaderLocation(shader, (p + "enabled").c_str()),
                ::GetShaderLocation(shader, (p + "type").c_str()),
                ::GetShaderLocation(shader, (p + "position").c_str()),
                ::GetShaderLocation(shader, (p + "target").c_str()),
                ::GetShaderLocation(shader, (p + "color").c_str()),
            };
        }
    }

    void setView(const raylib::Vector3 &viewPos) const {
        ::SetShaderValue(shader, viewPosLoc, &viewPos, SHADER_UNIFORM_VEC3);
    }

    void setLights() const {
        for (int i = 0; i < MAX_LIGHTS; i++) {
            const Light &l = lights[i];
            const LightLocs &loc = lightLocs[i];

            int enabled = l.enabled ? 1 : 0;
            int type = static_cast<int>(l.type);
            ::Vector4 color = ColorNormalize(l.color);

            ::SetShaderValue(shader, loc.enabled, &enabled, SHADER_UNIFORM_INT);
            ::SetShaderValue(shader, loc.type, &type, SHADER_UNIFORM_INT);
            ::SetShaderValue(
                shader, loc.position, &l.position, SHADER_UNIFORM_VEC3
            );
            ::SetShaderValue(
                shader, loc.target, &l.target, SHADER_UNIFORM_VEC3
            );
            ::SetShaderValue(shader, loc.color, &color, SHADER_UNIFORM_VEC4);
        }
    }

    void setMaterial(const Material &mat) const {
        ::SetShaderValue(shader, kaLoc, &mat.ka, SHADER_UNIFORM_VEC3);
        ::SetShaderValue(shader, kdLoc, &mat.kd, SHADER_UNIFORM_VEC3);
        ::SetShaderValue(shader, ksLoc, &mat.ks, SHADER_UNIFORM_VEC3);
        ::SetShaderValue(shader, sLoc, &mat.s, SHADER_UNIFORM_FLOAT);
    }

    void bind(const ::Model &model) const {
        for (int i = 0; i < model.materialCount; i++) {
            model.materials[i].shader = shader;
        }
    }

    void beginMode(const DrawContext &ctx) const {
        setView(ctx.camera.position);
        setLights();
    }

    void draw(const DrawContext &ctx) const;

    void endMode() const {
    }
};