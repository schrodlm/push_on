#include "Enemy.h"
#include <cmath>

Enemy::Enemy(Vector2 position, float health)
    : Entity(position, 15.0f)
    , m_target(position)
    , m_speed(120.0f)
    , m_health(health)
    , m_maxHealth(health)
    , m_contactDamage(10.0f)
{
}

void Enemy::Update(float deltaTime)
{
    if (!m_alive) return;

    // Move toward target (player)
    Vector2 direction = {
        m_target.x - m_position.x,
        m_target.y - m_position.y
    };

    // Normalize
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude > 5.0f)  // Stop when close enough
    {
        direction.x /= magnitude;
        direction.y /= magnitude;

        m_position.x += direction.x * m_speed * deltaTime;
        m_position.y += direction.y * m_speed * deltaTime;
    }
}

void Enemy::Draw() const
{
    if (m_alive)
    {
        // Draw enemy
        DrawCircleV(m_position, m_radius, RED);

        // Draw health bar
        float barWidth = 40.0f;
        float barHeight = 4.0f;
        float healthPercent = m_health / m_maxHealth;

        Vector2 barPos = { m_position.x - barWidth / 2, m_position.y - m_radius - 10.0f };
        DrawRectangle(barPos.x, barPos.y, barWidth, barHeight, DARKGRAY);
        DrawRectangle(barPos.x, barPos.y, barWidth * healthPercent, barHeight, RED);
    }
}

void Enemy::TakeDamage(float damage)
{
    m_health -= damage;
    if (m_health <= 0.0f)
    {
        m_health = 0.0f;
        Kill();
    }
}
