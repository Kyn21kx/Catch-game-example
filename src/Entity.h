#pragma once
#include "Vector2.h"
#include <cstdint>
#include <functional>

enum class EEntityType : uint8_t {
	Player,
	Enemy
};

// DOD (Data oriented design)
// Array of structs (ECS) / Struct of Arrays (ECS-invertido) / Array of ONE struct
struct Entity {
	EEntityType type;
	Game::Vector2 position;
	std::function<void(Entity* self)> draw;
	float moveSpeed = 1.f;
};

