#include "Player.h"
#include "EntityManager.h"
#include "Weapon.h"
#include "Logger.h"
#include <cmath>
#include <algorithm>

Player::Player(Vector2 position, int playerNumber)
    : Entity(position, 20.0f,
             1 << playerNumber,  // LAYER_PLAYER_1, LAYER_PLAYER_2, etc.
             LAYER_ENEMY | LAYER_ENEMY_ATTACK | LAYER_NEUTRAL_HAZARD | LAYER_PICKUP)
    , m_playerNumber(playerNumber)
    , m_speed(300.0f)
    , m_health(100.0f)
    , m_maxHealth(100.0f)
{
}

void Player::Update(float deltaTime)
{
    HandleInput(deltaTime);

    // Update weapon
    if (m_weapon) {
        m_weapon->Update(this, deltaTime);
    }
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
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();
        Shoot(mousePos);
    }
}

void Player::Shoot(Vector2 target)
{
    if (!m_weapon) {
        Logger::Debug("Player has no weapon equipped");
        return;
    }

    if (m_weapon->CanFire()) {
        m_weapon->Fire(this, target);
    }
}

void Player::Draw() const
{
    if (m_alive)
    {
        // Draw player with different colors per player number
        Color playerColors[] = { BLUE, GREEN, PURPLE, ORANGE };
        Color playerColor = playerColors[m_playerNumber % 4];

        DrawCircleV(m_position, m_radius, playerColor);

        // Calculate aim direction
        Vector2 mousePos = GetMousePosition();
        Vector2 aimDir = {
            mousePos.x - m_position.x,
            mousePos.y - m_position.y
        };
        float magnitude = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);
        if (magnitude > 0.0f) {
            aimDir.x /= magnitude;
            aimDir.y /= magnitude;
        }

        // Draw weapon if equipped
        if (m_weapon) {
            m_weapon->Draw(m_position, aimDir);
        }

        // Draw direction indicator
        DrawLineEx(m_position, mousePos, 2.0f, Fade(playerColor, 0.3f));

        // Draw health bar above player
        float barWidth = 50.0f;
        float barHeight = 5.0f;
        float healthPercent = m_health / m_maxHealth;

        Vector2 barPos = { m_position.x - barWidth / 2, m_position.y - m_radius - 15.0f };
        DrawRectangle(barPos.x, barPos.y, barWidth, barHeight, DARKGRAY);
        DrawRectangle(barPos.x, barPos.y, barWidth * healthPercent, barHeight, GREEN);
    }
}

void Player::OnCollision(Entity* other)
{
    if (!other) {
        return;
    }

    // Handle collision with enemies (contact damage)
    if (other->GetCollisionLayer() & LAYER_ENEMY)
    {
        TakeDamage(other->GetDamage());
    }

    // Handle collision with enemy attacks (projectiles, melee swings, etc)
    if (other->GetCollisionLayer() & LAYER_ENEMY_ATTACK)
    {
        TakeDamage(other->GetDamage());
    }

    // Handle collision with neutral hazards
    if (other->GetCollisionLayer() & LAYER_NEUTRAL_HAZARD)
    {
        TakeDamage(other->GetDamage());
    }

    // Handle collision with pickups
    if (other->GetCollisionLayer() & LAYER_PICKUP)
    {
        // Handle pickup collection
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
