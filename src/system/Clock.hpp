#pragma once

#include <Functions.hpp>
#include <Window.hpp>

class Clock {
  private:
    const raylib::Window &window;

    double targetFPS;
    double targetPPS;
    double graphicsDeltaTime;
    double physicsDeltaTime;

    double start;

  public:
    Clock(const raylib::Window &window, double fps, double pps)
        : window(window), targetFPS(1.0 / fps), targetPPS(1.0 / pps),
          graphicsDeltaTime(targetFPS * 2), physicsDeltaTime(targetPPS * 2) {
    }

    void startLoop() {
        start = window.GetTime();
    }

    void stopLoop() {
        double end = window.GetTime();
        double dt = end - start;
        double wait =
            std::min(
                targetFPS - graphicsDeltaTime, targetPPS - physicsDeltaTime
            ) -
            dt;
        if (wait > 0) {
            WaitTime(wait);
            end = window.GetTime();
            dt = end - start;
        }
        graphicsDeltaTime += dt;
        physicsDeltaTime += dt;
    }

    bool readyPhysics() {
        return physicsDeltaTime >= targetPPS;
    }

    double deltaPhysics() {
        return targetPPS;
    }

    void resetPhysics() {
        physicsDeltaTime = 0;
    }

    bool readyGraphics() {
        return graphicsDeltaTime >= targetFPS;
    }

    double deltaGraphics() {
        return targetFPS;
    }

    void resetGraphics() {
        graphicsDeltaTime = 0;
    }
};