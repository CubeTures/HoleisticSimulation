#pragma once

#include <variant>

#include "colliders/MeshCollider.hpp"
#include "colliders/NoneCollider.hpp"
#include "colliders/PlaneCollider.hpp"
#include "colliders/SphereCollider.hpp"

using Collider =
    std::variant<NoneCollider, SphereCollider, PlaneCollider, MeshCollider>;
