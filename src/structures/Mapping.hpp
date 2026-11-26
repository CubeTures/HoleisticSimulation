#pragma once

#include <string>

// forward reference
struct Arena;

struct Mapping {
    std::string asset;
    int entity;
    int collider;
    int graphic;

	void draw(Arena *arena);
};