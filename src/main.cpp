#include "Entity.h"
#include "Vector2.h"
#include "raylib.h"
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>

constexpr int32_t WIDTH = 1080;
constexpr int32_t HEIGHT = 720;

constexpr Color PLAYER_COLOR = BLACK;
constexpr Color ENEMY_COLOR = RED;
constexpr int32_t MORTAL_RADIUS = 80;
constexpr size_t PLAYER_INDEX = 0;
constexpr int32_t DISTANCE_IF_PLAYER = 1000;

#define PRINT_VEC(a) printf("(%.2f, %.2f)\n", a.x, a.y)

bool g_isGameOver = false;
float g_time = 0.f;

std::vector<Entity> g_entities;

float GetVectorMagnitude(const Game::Vector2& vector) {
    return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

Game::Vector2 NormalizedVector(const Game::Vector2& vector) {
    float mag = GetVectorMagnitude(vector);
    return Game::Vector2(vector.x / mag, vector.y / mag);
}

Game::Vector2 GetDirectionTo(const Game::Vector2& from, const Game::Vector2& to) {
    // Tienen magnitud 1
    return NormalizedVector(to - from);
}

void HandlePlayer(Entity* entity, float deltaTime) {
    if (g_isGameOver || entity->type != EEntityType::Player) return;

    Game::Vector2 velocity = Game::VEC2_ZERO;
    
    if (IsKeyDown(KEY_W)) {
        velocity.y -= 1;
    }
    if (IsKeyDown(KEY_S)) {
        velocity.y += 1;
    }
    if (IsKeyDown(KEY_A)) {
        velocity.x -= 1;
    }
    if (IsKeyDown(KEY_D)) {
        velocity.x += 1;
    }


    if (velocity.x == 0.f && velocity.y == 0.f) {
        return;
    }

    velocity = NormalizedVector(velocity);
    
    velocity = velocity * entity->moveSpeed * deltaTime;
    
    entity->position = entity->position + velocity;
    
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
        },
        .moveSpeed = 200.f
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

float CalculateDistanceWithPlayer(const Entity* entity){
    Game::Vector2 playerPos = g_entities[PLAYER_INDEX].position;

    if(entity->type == EEntityType::Player){
        return DISTANCE_IF_PLAYER;
    }

    float horizontalDistance = entity->position.x - playerPos.x;
    float verticalDistance = entity->position.y - playerPos.y;

    float sqrtA = sqrtf(horizontalDistance*horizontalDistance);
    float sqrtB = sqrtf(verticalDistance*verticalDistance);

    float distanceWithPlayer = sqrtA + sqrtB;
    return distanceWithPlayer;
}

void HandleGameOver(float distanceWithPlayer){
    if(distanceWithPlayer <= MORTAL_RADIUS){
        g_isGameOver = true;
        ClearBackground(RED);
        DrawText("GAME OVER", WIDTH / 2.f, HEIGHT * 0.2f, 48, LIGHTGRAY);
    }
}

void HandleRestartGame(){
    if(g_isGameOver && IsKeyDown(KEY_R)){
        ClearBackground(BLUE);
        g_entities.clear();
        SpawnPlayer();
        SpawnEnemy();
        g_isGameOver = false;
    }
}

void HandleEnemy(Entity* entity, float deltaTime) {
    if (entity->type != EEntityType::Enemy) return;
    // Find the player
    const Entity& player = g_entities[PLAYER_INDEX];
    const Game::Vector2& playerPos = player.position;
    
    // Find the direction
    Game::Vector2 direction = GetDirectionTo(entity->position, playerPos);
    entity->position = entity->position + (direction * entity->moveSpeed * deltaTime);
}

void Update(Entity* entity, float deltaTime) {
    float distanceWithPlayer = CalculateDistanceWithPlayer(entity);
    HandleGameOver(distanceWithPlayer);
    HandleRestartGame();
    // Todas las updates a las entidades ANTES de dibujarlas
    HandleEnemy(entity, deltaTime);
    HandlePlayer(entity, deltaTime);
    
    if (entity->position.x > WIDTH) {
        entity->position.x = 0.f;
    }
    else if (entity->position.x < 0.f) {
        entity->position.x = WIDTH;
    }
    if (entity->position.y > HEIGHT) {
        entity->position.y = 0.f;
    }
    else if (entity->position.y < 0.f) {
        entity->position.y = HEIGHT;
    }

    entity->draw(entity);
}


int main(void) {
    Init();
    while (!WindowShouldClose()) {
        BeginDrawing();
        // DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        ClearBackground(BLUE);
        float deltaTime = GetFrameTime();
        g_time += deltaTime;
        if (g_isGameOver) {
            deltaTime *= 0.f;
        }
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
