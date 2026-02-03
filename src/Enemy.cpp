#include "Enemy.h"
#include "Bullet.h"
#include "Weapon.h"
#include <cmath>

Enemy::Enemy(Vector2 position, float health, bool canHitOtherEnemies)
    : Entity(position, 15.0f,
             LAYER_ENEMY,
             LAYER_PLAYER_ATTACK | LAYER_NEUTRAL_HAZARD |
             (canHitOtherEnemies ? LAYER_ENEMY_ATTACK : 0))
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

    // Update weapon
    if (m_weapon) {
        m_weapon->Update(deltaTime);

        // Attack if in range
        if (magnitude < 200.0f && m_weapon->CanFire()) {
            m_weapon->Fire(this, m_target);
        }
    }
}

void Enemy::Draw() const
{
    if (m_alive)
    {
        // Draw enemy
        DrawCircleV(m_position, m_radius, RED);

        // Calculate aim direction toward target
        Vector2 aimDir = {
            m_target.x - m_position.x,
            m_target.y - m_position.y
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

        // Draw health bar
        float barWidth = 40.0f;
        float barHeight = 4.0f;
        float healthPercent = m_health / m_maxHealth;

        Vector2 barPos = { m_position.x - barWidth / 2, m_position.y - m_radius - 10.0f };
        DrawRectangle(barPos.x, barPos.y, barWidth, barHeight, DARKGRAY);
        DrawRectangle(barPos.x, barPos.y, barWidth * healthPercent, barHeight, RED);
    }
}

void Enemy::OnCollision(Entity* other)
{
    // Handle collision with player attacks
    if (other->GetCollisionLayer() & LAYER_PLAYER_ATTACK)
    {
        if (auto* bullet = dynamic_cast<Bullet*>(other))
        {
            TakeDamage(bullet->GetDamage());
        }
    }

    // Handle collision with enemy attacks (if enabled)
    if (other->GetCollisionLayer() & LAYER_ENEMY_ATTACK)
    {
        // Only take damage if we're set to collide with enemy attacks
        if (m_collisionMask & LAYER_ENEMY_ATTACK)
        {
            if (auto* bullet = dynamic_cast<Bullet*>(other))
            {
                TakeDamage(bullet->GetDamage());
            }
        }
    }

    // Handle collision with neutral hazards
    if (other->GetCollisionLayer() & LAYER_NEUTRAL_HAZARD)
    {
        // Handle hazard collision
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
