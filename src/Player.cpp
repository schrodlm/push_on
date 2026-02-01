#include "Player.h"
#include <cmath>
#include <algorithm>

Player::Player(Vector2 position)
    : Entity(position, 20.0f)
    , m_speed(300.0f)
    , m_health(100.0f)
    , m_maxHealth(100.0f)
    , m_shootCooldown(0.15f)  // 150ms between shots
    , m_shootCooldownTimer(0.0f)
{
}

void Player::Update(float deltaTime)
{
    HandleInput(deltaTime);

    // Update shoot cooldown
    if (m_shootCooldownTimer > 0.0f)
    {
        m_shootCooldownTimer -= deltaTime;
    }

    // Update bullets
    for (auto& bullet : m_bullets)
    {
        bullet->Update(deltaTime);
    }

    // Remove dead bullets
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const std::unique_ptr<Bullet>& bullet) {
                return !bullet->IsAlive();
            }),
        m_bullets.end()
    );
}

void Player::HandleInput(float deltaTime)
{
    // Movement (WASD)
    Vector2 movement = { 0.0f, 0.0f };
    if (IsKeyDown(KEY_W)) movement.y -= 1.0f;
    if (IsKeyDown(KEY_S)) movement.y += 1.0f;
    if (IsKeyDown(KEY_A)) movement.x -= 1.0f;
    if (IsKeyDown(KEY_D)) movement.x += 1.0f;

    // Normalize diagonal movement
    float magnitude = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (magnitude > 0.0f)
    {
        movement.x /= magnitude;
        movement.y /= magnitude;
    }

    // Apply movement
    m_position.x += movement.x * m_speed * deltaTime;
    m_position.y += movement.y * m_speed * deltaTime;

    // Keep player in bounds
    m_position.x = std::clamp(m_position.x, m_radius, 1280.0f - m_radius);
    m_position.y = std::clamp(m_position.y, m_radius, 720.0f - m_radius);

    // Shooting
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && m_shootCooldownTimer <= 0.0f)
    {
        Vector2 mousePos = GetMousePosition();
        Shoot(mousePos);
        m_shootCooldownTimer = m_shootCooldown;
    }
}

void Player::Shoot(Vector2 target)
{
    // Calculate direction to target
    Vector2 direction = {
        target.x - m_position.x,
        target.y - m_position.y
    };

    // Normalize
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude > 0.0f)
    {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }

    // Bullet velocity
    float bulletSpeed = 800.0f;
    Vector2 velocity = {
        direction.x * bulletSpeed,
        direction.y * bulletSpeed
    };

    // Spawn bullet slightly ahead of player
    Vector2 spawnPos = {
        m_position.x + direction.x * (m_radius + 10.0f),
        m_position.y + direction.y * (m_radius + 10.0f)
    };

    m_bullets.push_back(std::make_unique<Bullet>(spawnPos, velocity, 25.0f));
}

void Player::Draw() const
{
    if (m_alive)
    {
        // Draw player
        DrawCircleV(m_position, m_radius, BLUE);

        // Draw direction indicator (line to mouse)
        Vector2 mousePos = GetMousePosition();
        DrawLineEx(m_position, mousePos, 2.0f, Fade(BLUE, 0.3f));

        // Draw health bar above player
        float barWidth = 50.0f;
        float barHeight = 5.0f;
        float healthPercent = m_health / m_maxHealth;

        Vector2 barPos = { m_position.x - barWidth / 2, m_position.y - m_radius - 15.0f };
        DrawRectangle(barPos.x, barPos.y, barWidth, barHeight, DARKGRAY);
        DrawRectangle(barPos.x, barPos.y, barWidth * healthPercent, barHeight, GREEN);
    }

    // Draw bullets
    for (const auto& bullet : m_bullets)
    {
        bullet->Draw();
    }
}

void Player::TakeDamage(float damage)
{
    m_health -= damage;
    if (m_health <= 0.0f)
    {
        m_health = 0.0f;
        Kill();
    }
}
