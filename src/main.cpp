#include <Camera3D.hpp>
#include <Color.hpp>
#include <Functions.hpp>
#include <Window.hpp>

#include "arenas/Init.hpp"
#include "structures/Clock.hpp"

int main() {
    raylib::Window window = raylib::Window(1280, 720);
    raylib::Camera3D camera =
        raylib::Camera3D(raylib::Vector3(0, 0, -20), raylib::Vector3(0, 0, 1));
    Arena *arena = init();

    Clock clock(60, 600);
    while (!window.ShouldClose()) {
        clock.startLoop(window);

        if (clock.readyPhysics()) {
            // integration.integrate(clock.deltaPhysics(), entities);
            clock.resetPhysics();
        }

        if (clock.readyGraphics()) {
            window.BeginDrawing();
            window.ClearBackground(raylib::Color::Black());
            camera.BeginMode();

            for (Mapping m : arena->_map) {
                m.draw(arena);
            }

            camera.EndMode();
            window.EndDrawing();
            clock.resetGraphics();
        }

        clock.stopLoop(window);
    }

    window.Close();
    return 0;
}

/*
    Need to handle:
    forces (gravity, wind)
    rigid bodies
    particles, flocks
        - become generators allocate
    shaders
    input
    camera
    heightmap -> mesh
    bvh
    collisions
        -
    integration methods
        -
*/