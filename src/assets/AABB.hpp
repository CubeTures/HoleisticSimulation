#pragma once

#include <initializer_list>
#include <limits>

#include "Vector3.hpp"

class AABB {
  private:
    raylib::Vector3 pMin, pMax;

  public:
    AABB()
        : pMin(
              std::numeric_limits<float>::max(),
              std::numeric_limits<float>::max(),
              std::numeric_limits<float>::max()
          ),
          pMax(
              std::numeric_limits<float>::lowest(),
              std::numeric_limits<float>::lowest(),
              std::numeric_limits<float>::lowest()
          ) {
    }

    AABB(std::initializer_list<raylib::Vector3> points) : AABB() {
        for (const raylib::Vector3 p : points) {
            grow(p);
        }
    }

    void grow(raylib::Vector3 p) {
        if (p.x < pMin.x) {
            pMin.x = p.x;
        }
        if (p.y < pMin.y) {
            pMin.y = p.y;
        }
        if (p.z < pMin.z) {
            pMin.z = p.z;
        }
        if (p.x > pMax.x) {
            pMax.x = p.x;
        }
        if (p.y > pMax.y) {
            pMax.y = p.y;
        }
        if (p.z > pMax.z) {
            pMax.z = p.z;
        }
    }

    int longestAxis() {
        float x = pMax.x - pMin.x;
        float y = pMax.y - pMin.y;
        float z = pMax.z - pMin.z;

        if (x >= y && x >= z) {
            return 0;
        } else if (y >= z) {
            return 1;
        } else {
            return 2;
        }
    }

    bool flatAlong(int axis) {
        switch (axis) {
        case 0:
            return pMax.x - pMin.x < EPSILON;
        case 1:
            return pMax.y - pMin.y < EPSILON;
        default:
            return pMax.z - pMin.z < EPSILON;
        }
    }

    bool overlaps(const AABB &other) const {
        return pMin.x <= other.pMax.x && pMax.x >= other.pMin.x &&
               pMin.y <= other.pMax.y && pMax.y >= other.pMin.y &&
               pMin.z <= other.pMax.z && pMax.z >= other.pMin.z;
    }

    raylib::Vector3 center() const {
        return (pMax + pMin) / 2;
    }

    raylib::Vector3 half() const {
        return (pMax - pMin) / 2;
    }

    float volume() const {
        return (pMax.x - pMin.x) * (pMax.y - pMin.y) * (pMax.z - pMin.z);
    }
};