#include "Systems.h"
#include "core/Entity.h"
#include "core/Vector2.h"
#include "raylib.h"

constexpr int32_t WIDTH = 1080;
constexpr int32_t HEIGHT = 720;

void Systems::Init() {
	// Initial position
	Entity player = {
		.type = EEntityType::Player,
		.position = Game::Vector2(WIDTH / 2.f, HEIGHT / 2.f),
		.velocity = Game::VEC2_ZERO,
		.draw = [](Entity* self){
			DrawCircle(self->position.x, self->position.y, 20.f, YELLOW);
		},
		.moveSpeed = 0.f
	};
	g_entities.push_back(player);
}

void Systems::PlayerUpdate(Entity &entity, float deltaTime) {
	// Gravity
	constexpr float gravityForce = 30.f;
	entity.velocity = Game::VEC2_DOWN * gravityForce;

}


void Systems::EntityDraw(Entity &entity) {
	entity.draw(&entity);
}
