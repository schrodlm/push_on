#include "raylib.h"
#include <cmath>
#include <algorithm>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

int main(void)
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Roguelike Game");
    SetTargetFPS(TARGET_FPS);

    // Player setup
    Vector2 playerPos = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    float playerSpeed = 300.0f;
    float playerRadius = 20.0f;

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        float deltaTime = GetFrameTime();

        // Player movement (WASD)
        Vector2 movement = { 0.0f, 0.0f };
        if (IsKeyDown(KEY_W)) movement.y -= 1.0f;
        if (IsKeyDown(KEY_S)) movement.y += 1.0f;
        if (IsKeyDown(KEY_A)) movement.x -= 1.0f;
        if (IsKeyDown(KEY_D)) movement.x += 1.0f;

        // Normalize diagonal movement
        float magnitude = sqrtf(movement.x * movement.x + movement.y * movement.y);
        if (magnitude > 0.0f)
        {
            movement.x /= magnitude;
            movement.y /= magnitude;
        }

        // Apply movement
        playerPos.x += movement.x * playerSpeed * deltaTime;
        playerPos.y += movement.y * playerSpeed * deltaTime;

        // Keep player in bounds
        playerPos.x = std::clamp(playerPos.x, playerRadius, SCREEN_WIDTH - playerRadius);
        playerPos.y = std::clamp(playerPos.y, playerRadius, SCREEN_HEIGHT - playerRadius);

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Draw player
        DrawCircleV(playerPos, playerRadius, BLUE);

        // Draw crosshair at mouse position
        Vector2 mousePos = GetMousePosition();
        DrawLineV((Vector2){ mousePos.x - 10, mousePos.y },
                  (Vector2){ mousePos.x + 10, mousePos.y }, RED);
        DrawLineV((Vector2){ mousePos.x, mousePos.y - 10 },
                  (Vector2){ mousePos.x, mousePos.y + 10 }, RED);

        // Draw UI
        DrawFPS(10, 10);
        DrawText("WASD to move | Mouse to aim", 10, 40, 20, LIGHTGRAY);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
