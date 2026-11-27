#pragma once

#include <variant>

#include <Vector3.hpp>

struct NoneCollider {};
struct SphereCollider {};
struct PlaneCollider {};
struct MeshCollider {};

using Collider =
    std::variant<NoneCollider, SphereCollider, PlaneCollider, MeshCollider>;
