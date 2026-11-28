#include <cstdio>
#include <utility>

#include <Camera3D.hpp>
#include <Color.hpp>
#include <Functions.hpp>
#include <Window.hpp>

#include "arenas/Init.hpp"
#include "shaders/Renderer.hpp"
#include "system/Clock.hpp"
#include "system/Controller.hpp"

int main() {
    raylib::Window window(1280, 720);
    raylib::Camera3D camera(
        raylib::Vector3(0, 0, -20), raylib::Vector3(0, 0, 1)
    );
    Controller controller(camera, 10);

    Arena *arena = init();
    Renderer renderer(BlinnPhongShader{}, ParticleShader{});
    Clock clock(window, 60, 600);

    while (!window.ShouldClose()) {
        clock.startLoop();

        if (clock.readyPhysics()) {
            arena->stepForces(clock.deltaPhysics());
            arena->updateCandidates();
            arena->updateContacts();
            arena->resolveContacts();
            std::swap(arena->entities, arena->next);
            clock.resetPhysics();
        }

        if (clock.readyGraphics()) {
            controller.listen(clock.deltaGraphics());

            window.BeginDrawing();
            window.ClearBackground(raylib::Color::Black());
            camera.BeginMode();

            renderer.draw(*arena, camera);

            camera.EndMode();
            window.EndDrawing();
            clock.resetGraphics();
        }

        clock.stopLoop();
    }

    window.Close();
    return 0;
}
