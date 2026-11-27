#pragma once

#include "Vector3.hpp"

struct Triangle {
    raylib::Vector3 a, b, c;

    raylib::Vector3 normal() const {
        return (b - a).CrossProduct(c - a).Normalize();
    }

    raylib::Vector3 barycentric(raylib::Vector3 p) const {
        raylib::Vector3 v0 = b - a;
        raylib::Vector3 v1 = c - a;
        raylib::Vector3 v2 = p - a;

        float d00 = v0.DotProduct(v0);
        float d01 = v0.DotProduct(v1);
        float d11 = v1.DotProduct(v1);
        float d20 = v2.DotProduct(v0);
        float d21 = v2.DotProduct(v1);

        float denom = d00 * d11 - d01 * d01;
        float v = (d11 * d20 - d01 * d21) / denom;
        float w = (d00 * d21 - d01 * d20) / denom;
        float u = 1 - v - w;

        return raylib::Vector3(u, v, w);
    }

    std::pair<float, float> barycentricEdge(
        raylib::Vector3 a,
        raylib::Vector3 b,
        raylib::Vector3 p
    ) const {
        raylib::Vector3 ab = b - a;
        float t = (p - a).DotProduct(ab) / ab.DotProduct(ab);
        return {1 - t, t};
    }

    // https://cedarcantab.wordpress.com/2021/02/21/javascript-physics-engine-from-scratch-closest-point-to-a-triangle-barycentric-coordinates-method/
    raylib::Vector3 closestPoint(raylib::Vector3 p) const {
        raylib::Vector3 bary = barycentric(p);
        auto [uab, vab] = barycentricEdge(a, b, p);
        auto [ubc, vbc] = barycentricEdge(b, c, p);
        auto [uca, vca] = barycentricEdge(c, a, p);

        if (vab <= 0 && uca <= 0) {
            return a;
        } else if (uab <= 0 && vbc <= 0) {
            return b;
        } else if (vca <= 0 && ubc <= 0) {
            return c;
        } else if (uab >= 0 && vab >= 0 && bary.z <= 0) {
            return a * uab + b * vab;
        } else if (ubc >= 0 && vbc >= 0 && bary.x <= 0) {
            return b * ubc + c * vbc;
        } else if (uca >= 0 && vca >= 0 && bary.y <= 0) {
            return c * uca + a * vca;
        } else {
            return a * bary.x + b * bary.y + c * bary.z;
        }
    }

    static bool inside(raylib::Vector3 bc) {
        return bc.x >= 0 && bc.x <= 1 && bc.y >= 0 && bc.y <= 1 && bc.z >= 0 &&
               bc.z <= 1;
    }
};