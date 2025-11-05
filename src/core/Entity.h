#pragma once
#include "Vector2.h"
#include <cstdint>
#include <functional>
#include <vector>

enum class EEntityType : uint8_t {
	Player,
	Enemy
};

// DOD (Data oriented design)
// Array of structs (ECS) / Struct of Arrays (ECS-invertido) / Array of ONE struct
struct Entity {
	EEntityType type;
	Game::Vector2 position;
	Game::Vector2 velocity;
	std::function<void(Entity* self)> draw;
	float moveSpeed = 1.f;
};

// Vivo durante toda la app
inline std::vector<Entity> g_entities;


