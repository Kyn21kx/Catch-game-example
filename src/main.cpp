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
constexpr int32_t PLAYER_INDEX = 0;
constexpr int32_t DISTANCE_IF_PLAYER = 1000;

#define PRINT_VEC(a) printf("(%.2f, %.2f)\n", a.x, a.y)

bool g_isGameOver = false;

std::vector<Entity> g_entities;

float GetVectorMagnitude(const Game::Vector2& vector) {
    return sqrtf((vector.x * vector.x) + (vector.y * vector.y));
}

Game::Vector2 NormalizedVector(const Game::Vector2& vector) {
    float mag = GetVectorMagnitude(vector);
    return Game::Vector2(vector.x / mag, vector.y / mag);
}


void HandleInput(Entity* entity, float deltaTime) {
    if(g_isGameOver) return;
    if (entity->type != EEntityType::Player) return;

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

    printf("La input del usuario raw: ");
    PRINT_VEC(velocity);

    if (velocity.x == 0.f && velocity.y == 0.f) {
        return;
    }

    velocity = NormalizedVector(velocity);
    printf("La input del usuario normalizada: ");
    PRINT_VEC(velocity);
    
    velocity = velocity * entity->moveSpeed * deltaTime;
    printf("La velocidad que se le agrega a position: ");
    PRINT_VEC(velocity);
    
    printf("La posicion inicial: ");
    PRINT_VEC(entity->position);
    entity->position = entity->position + velocity;
    
    printf("La posicion final: ");
    PRINT_VEC(entity->position);
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


void Update(Entity* entity, float deltaTime) {
    if (entity->type == EEntityType::Player) {
    }
    
    float distanceWithPlayer = CalculateDistanceWithPlayer(entity);
    HandleGameOver(distanceWithPlayer);
    HandleRestartGame();
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
