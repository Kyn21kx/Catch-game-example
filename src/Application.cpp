#include "Application.h"
#include "core/Entity.h"
#include "Systems.h"
#include "raylib.h"
#include <cstdint>

constexpr int32_t WIDTH = 1080;
constexpr int32_t HEIGHT = 720;

float g_time = 0.f;

void Application::Init() {
    InitWindow(WIDTH, HEIGHT, "Game example");
    SetTargetFPS(60);
	g_entities.reserve(100);
	Systems::Init();
}


void Application::Run() {
	while (!WindowShouldClose()) {
        BeginDrawing();
        // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        ClearBackground(BLUE);
        float deltaTime = GetFrameTime();
        g_time += deltaTime;

        for (Entity& entity : g_entities) {
        	entity.position = entity.position + (entity.velocity * deltaTime);
        	if (entity.type == EEntityType::Player) {
        		Systems::PlayerUpdate(entity, deltaTime);
        	}
        	Systems::EntityDraw(entity);
        }

        EndDrawing();
    }

    CloseWindow();
}

void Application::Exit() {
	
}

