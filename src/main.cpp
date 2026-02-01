#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

int main(void)
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Push On");
    SetTargetFPS(TARGET_FPS);

    // Create player
    Player player({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f });

    // Create some test enemies
    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Enemy>(Vector2{ 200.0f, 200.0f }));
    enemies.push_back(std::make_unique<Enemy>(Vector2{ 1080.0f, 200.0f }));
    enemies.push_back(std::make_unique<Enemy>(Vector2{ 640.0f, 500.0f }));

    int enemiesKilled = 0;

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // Update player
        player.Update(deltaTime);

        // Update enemies (they follow the player)
        for (auto& enemy : enemies)
        {
            if (enemy->IsAlive())
            {
                enemy->SetTarget(player.GetPosition());
                enemy->Update(deltaTime);
            }
        }

        // Collision detection: Bullets vs Enemies
        auto& bullets = player.GetBullets();
        for (auto& bullet : bullets)
        {
            if (!bullet->IsAlive()) continue;

            for (auto& enemy : enemies)
            {
                if (!enemy->IsAlive()) continue;

                if (bullet->CollidesWith(*enemy))
                {
                    enemy->TakeDamage(bullet->GetDamage());
                    bullet->Kill();

                    if (!enemy->IsAlive())
                    {
                        enemiesKilled++;
                    }
                    break;  // Bullet can only hit one enemy
                }
            }
        }

        // Collision detection: Player vs Enemies (contact damage)
        for (auto& enemy : enemies)
        {
            if (enemy->IsAlive() && player.IsAlive())
            {
                if (player.CollidesWith(*enemy))
                {
                    player.TakeDamage(enemy->GetDamage() * deltaTime);  // Continuous damage
                }
            }
        }

        // Remove dead enemies
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::unique_ptr<Enemy>& enemy) {
                    return !enemy->IsAlive();
                }),
            enemies.end()
        );

        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Draw entities
        player.Draw();
        for (const auto& enemy : enemies)
        {
            enemy->Draw();
        }

        // Draw crosshair at mouse position
        Vector2 mousePos = GetMousePosition();
        DrawLineV((Vector2){ mousePos.x - 10, mousePos.y },
                  (Vector2){ mousePos.x + 10, mousePos.y }, RED);
        DrawLineV((Vector2){ mousePos.x, mousePos.y - 10 },
                  (Vector2){ mousePos.x, mousePos.y + 10 }, RED);

        // Draw UI
        DrawFPS(10, 10);
        DrawText("WASD: Move | Left Click: Shoot", 10, 40, 20, LIGHTGRAY);

        std::string healthText = "Health: " + std::to_string(static_cast<int>(player.GetHealth()));
        DrawText(healthText.c_str(), 10, 70, 20, GREEN);

        std::string killText = "Enemies Killed: " + std::to_string(enemiesKilled);
        DrawText(killText.c_str(), 10, 100, 20, YELLOW);

        if (!player.IsAlive())
        {
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, 60, RED);
            DrawText("Press ESC to exit", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 70, 30, WHITE);
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
