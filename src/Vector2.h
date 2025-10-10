#pragma once
#include <complex>
namespace Game {
	class Vector2 {
	public:
		float x;
		float y;


		constexpr Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}

		Vector2 operator* (float scalar) {
			return Vector2(this->x * scalar, this->y * scalar);
		}
		
		Vector2 operator+ (const Vector2& other) {
			return Vector2(this->x + other.x, this->y + other.y);
		}
	
	};
	constexpr Vector2 VEC2_ZERO = Vector2(0.f,0.f);
}

