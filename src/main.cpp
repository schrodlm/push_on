#include "EntityManager.h"
#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include <memory>
#include <string>
#include "Logger.h"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

int main(void)
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Push On");
    SetTargetFPS(TARGET_FPS);

    // Get EntityManager instance
    EntityManager& manager = EntityManager::getInstance();

    // Create player (player number 0)
    manager.queueEntity(std::make_unique<Player>(
        Vector2{ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
        0  // Player 1
    ));

    // Create enemies
    manager.queueEntity(std::make_unique<Enemy>(Vector2{ 200.0f, 200.0f }, 50.0f, false));
    manager.queueEntity(std::make_unique<Enemy>(Vector2{ 1080.0f, 200.0f }, 50.0f, false));
    manager.queueEntity(std::make_unique<Enemy>(Vector2{ 640.0f, 500.0f }, 50.0f, false));

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Update enemy AI - enemies chase closest player (clean, no casting!)
        for (Enemy* enemy : manager.getEnemies()) {
            if (Player* target = manager.getClosestPlayer(enemy->GetPosition())) {
                enemy->SetTarget(target->GetPosition());
            }
        }
        manager.addWaitingEntities();
        // Update all entities (movement, AI, etc)
        manager.updateEntities(deltaTime);
        // Check collisions (spatial hash + layer filtering)
        manager.checkCollisions();

        // Remove dead entities
        manager.deleteDeadEntities();

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Draw all entities
        manager.drawEntities();

        // Draw crosshair at mouse position
        Vector2 mousePos = GetMousePosition();
        DrawLineV((Vector2){ mousePos.x - 10, mousePos.y },
                  (Vector2){ mousePos.x + 10, mousePos.y }, RED);
        DrawLineV((Vector2){ mousePos.x, mousePos.y - 10 },
                  (Vector2){ mousePos.x, mousePos.y + 10 }, RED);

        // Draw UI
        DrawFPS(10, 10);
        DrawText("WASD: Move | Left Click: Shoot", 10, 40, 20, LIGHTGRAY);

        // Display player info (clean API, no casting!)
        if (Player* player = manager.getPlayer(0)) {
            std::string healthText = "Health: " + std::to_string(static_cast<int>(player->GetHealth()));
            DrawText(healthText.c_str(), 10, 70, 20, GREEN);

            if (!player->IsAlive()) {
                DrawText("GAME OVER", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, 60, RED);
                DrawText("Press ESC to exit", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 70, 30, WHITE);
            }
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
