#include "EntityManager.h"
#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include "Gun.h"
#include "Sword.h"
#include "WeaponPickup.h"
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Logger.h"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

// Helper function to create random weapon
std::unique_ptr<Weapon> CreateRandomWeapon() {
    int choice = rand() % 2;
    if (choice == 0) {
        return std::make_unique<Gun>();
    } else {
        return std::make_unique<Sword>();
    }
}

int main(void)
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Push On");
    SetTargetFPS(TARGET_FPS);
    srand(time(nullptr));  // Seed random number generator

    // Get EntityManager instance
    EntityManager& manager = EntityManager::getInstance();

    // Create player (player number 0)
    manager.queueEntity(std::make_unique<Player>(
        Vector2{ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f },
        0  // Player 1
    ));

    // Create first enemy with random weapon
    auto firstEnemy = std::make_unique<Enemy>(Vector2{ 200.0f, 200.0f }, 100.0f, false);
    firstEnemy->EquipWeapon(CreateRandomWeapon());
    manager.queueEntity(std::move(firstEnemy));

    // Spawn test weapon pickup
    manager.queueEntity(std::make_unique<WeaponPickup>(
        Vector2{ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f },
        std::make_unique<Gun>()
    ));

    // Spawn sword pickup
    manager.queueEntity(std::make_unique<WeaponPickup>(
        Vector2{ SCREEN_WIDTH / 2.0f + 150.0f, SCREEN_HEIGHT / 2.0f - 100.0f },
        std::make_unique<Sword>()
    ));

    // Track if we need to spawn a new enemy
    bool hasActiveEnemy = true;

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Check if current enemy is dead and spawn new one
        if (!hasActiveEnemy) {
            Vector2 spawnPos = { 200.0f, 200.0f };
            auto newEnemy = std::make_unique<Enemy>(spawnPos, 100.0f, false);
            newEnemy->EquipWeapon(CreateRandomWeapon());
            manager.queueEntity(std::move(newEnemy));
            hasActiveEnemy = true;
            Logger::Info("Spawned new enemy with random weapon");
        }

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

        // Check if enemy is still alive
        hasActiveEnemy = !manager.getEnemies().empty();

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
