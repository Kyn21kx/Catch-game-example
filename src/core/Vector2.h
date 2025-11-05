#pragma once
namespace Game {
	class Vector2 {
	public:
		float x;
		float y;


		constexpr Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}

		Vector2 operator* (float scalar) const {
			return Vector2(this->x * scalar, this->y * scalar);
		}
		
		Vector2 operator+ (const Vector2& other) const {
			return Vector2(this->x + other.x, this->y + other.y);
		}
		
		Vector2 operator- (const Vector2& other) const {
			return Vector2(this->x - other.x, this->y - other.y);
		}
	
	};
	constexpr Vector2 VEC2_ZERO = Vector2(0.f,0.f);
	constexpr Vector2 VEC2_UP = Vector2(0.f,-1.f);
	constexpr Vector2 VEC2_DOWN = Vector2(0.f,1.f);
	constexpr Vector2 VEC2_RIGHT = Vector2(1.f,0.f);
	constexpr Vector2 VEC2_LEFT = Vector2(-1.f,0.f);
}

// std::vector<Entity> g_entities;

// float GetVectorMagnitude(const Game::Vector2& vector) {
//     return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
// }

// Game::Vector2 NormalizedVector(const Game::Vector2& vector) {
//     float mag = GetVectorMagnitude(vector);
//     return Game::Vector2(vector.x / mag, vector.y / mag);
// }

// Game::Vector2 GetDirectionTo(const Game::Vector2& from, const Game::Vector2& to) {
//     // Tienen magnitud 1
//     return NormalizedVector(to - from);
// }

// int32_t GetRandRange(int32_t min, int32_t max) {
//     return min + rand() % (max - min + 1);
// }
