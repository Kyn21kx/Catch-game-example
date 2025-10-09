#include "Entity.h"
#include "Vector2.h"
#include "raylib.h"
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>

constexpr int32_t WIDTH = 1080;
constexpr int32_t HEIGHT = 720;

constexpr Color PLAYER_COLOR = BLACK; 
constexpr Color ENEMY_COLOR = RED; 
constexpr int MORTAL_RADIUS = 80;
constexpr int PLAYER_INDEX = 0;

bool g_isGameOver = false;

std::vector<Entity> g_entities;



void HandleInput(Entity* entity, float deltaTime) {
    if(g_isGameOver == true) return;
    if (entity->type != EEntityType::Player) return;

    if (IsKeyDown(KEY_W)) {
        entity->position.y -= entity->moveSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_S)) {
        entity->position.y+= entity->moveSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_A)) {
        entity->position.x-= entity->moveSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_D)) {
        entity->position.x+= entity->moveSpeed * deltaTime;
    }
}

void SpawnPlayer() {
    Entity player = {
        .type = EEntityType::Player,
        .position = Game::Vector2(WIDTH / 2.f, HEIGHT / 2.f),
        .draw = [](Entity* self) {
            DrawCircle(self->position.x, self->position.y, 35, PLAYER_COLOR);
        },
        .moveSpeed = 500.f
    };
    g_entities.push_back(player);
}

void SpawnEnemy() {
    Entity enemy = {
        .type = EEntityType::Enemy,
        .position = Game::Vector2(WIDTH / 2.f, HEIGHT / 3.f),
        .draw = [](Entity* self) {
            DrawCircle(self->position.x, self->position.y, 35, ENEMY_COLOR);
        }
    };
    g_entities.push_back(enemy);
}

void Init() {
    InitWindow(WIDTH, HEIGHT, "Game example");
    SetTargetFPS(60);
    g_entities.reserve(100);
    SpawnPlayer();
    SpawnEnemy();
}

void CalculateDistanceWithPlayer(Entity* entity){
    Game::Vector2 playerPos = g_entities[PLAYER_INDEX].position;

    if(entity->type != EEntityType::Enemy){
        return;
    }

    float a = entity->position.x - playerPos.x;
    float b = entity->position.y - playerPos.y;

    float sqrtA = sqrt(a*a);
    float sqrtB = sqrt(b*b);

    float c = sqrtA + sqrtB;

    if(c <= MORTAL_RADIUS){
        g_isGameOver = true;
        ClearBackground(RED);
        DrawText("GAME OVER", WIDTH / 2.f, HEIGHT * 0.2f, 48, LIGHTGRAY);
    }
}


void Update(Entity* entity, float deltaTime) {
    CalculateDistanceWithPlayer(entity);
    // Todas las updates a las entidades ANTES de dibujarlas
    HandleInput(entity, deltaTime);
    entity->draw(entity);
}


int main(void) {
    Init();
    while (!WindowShouldClose()) {
        BeginDrawing();
        // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        ClearBackground(BLUE);
        float deltaTime = GetFrameTime();
        int fps = GetFPS();
        DrawText(std::to_string(fps).c_str(), WIDTH / 2.f, HEIGHT * 0.1f, 48, LIGHTGRAY);

        for (Entity& entity : g_entities) {
            Update(&entity, deltaTime);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
