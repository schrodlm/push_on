#include "Sword.h"
#include "EntityManager.h"
#include "Enemy.h"
#include "Logger.h"
#include <cmath>

Sword::Sword()
    : Weapon("Sword", 0.5f),  // 500ms cooldown
      m_damage(40.0f),
      m_range(60.0f),
      m_swingDuration(0.3f),
      m_swingTimer(0.0f),
      m_isSwinging(false),
      m_swingDirection({0.0f, 0.0f}) {
}

void Sword::Fire(Entity* owner, Vector2 target) {
    if (!CanFire() || !owner) return;

    Logger::Debug("Sword::Fire");
    m_isSwinging = true;
    m_swingTimer = m_swingDuration;
    m_cooldownTimer = m_cooldown;

    // Calculate swing direction
    Vector2 ownerPos = owner->GetPosition();
    m_swingDirection = {
        target.x - ownerPos.x,
        target.y - ownerPos.y
    };
    float magnitude = std::sqrt(m_swingDirection.x * m_swingDirection.x +
                                m_swingDirection.y * m_swingDirection.y);
    if (magnitude > 0.0f) {
        m_swingDirection.x /= magnitude;
        m_swingDirection.y /= magnitude;
    }

    // Check for hits in melee range
    Vector2 hitCenter = {
        ownerPos.x + m_swingDirection.x * m_range * 0.5f,
        ownerPos.y + m_swingDirection.y * m_range * 0.5f
    };

    // Hit all enemies in range
    for (auto* enemy : EntityManager::getInstance().getEnemies()) {
        if (!enemy || !enemy->IsAlive()) continue;

        Vector2 enemyPos = enemy->GetPosition();
        float dx = enemyPos.x - hitCenter.x;
        float dy = enemyPos.y - hitCenter.y;
        float distSquared = dx * dx + dy * dy;
        float hitRadius = m_range * 0.7f; // Generous hit area

        if (distSquared < hitRadius * hitRadius) {
            // Check if enemy is roughly in front of swing direction
            float dotProduct = dx * m_swingDirection.x + dy * m_swingDirection.y;
            if (dotProduct > 0) {
                Logger::Debug("Sword hit enemy for ", m_damage, " damage");
                enemy->TakeDamage(m_damage);
            }
        }
    }
}

void Sword::Update(float deltaTime) {
    // Call base class to update cooldown
    Weapon::Update(deltaTime);

    if (m_swingTimer > 0.0f) {
        m_swingTimer -= deltaTime;
        if (m_swingTimer <= 0.0f) {
            m_isSwinging = false;
        }
    }
}

void Sword::Draw(Vector2 ownerPos, Vector2 aimDir) const {
    if (m_isSwinging) {
        // Draw swing arc
        float swingProgress = 1.0f - (m_swingTimer / m_swingDuration);
        float baseAngle = std::atan2(m_swingDirection.y, m_swingDirection.x) * RAD2DEG;
        float swingAngle = -60.0f + swingProgress * 120.0f; // -60 to +60 degrees
        float currentAngle = baseAngle + swingAngle;

        Vector2 swordEnd = {
            ownerPos.x + std::cos(currentAngle * DEG2RAD) * m_range,
            ownerPos.y + std::sin(currentAngle * DEG2RAD) * m_range
        };

        DrawLineEx(ownerPos, swordEnd, 4.0f, Fade(WHITE, 0.8f));
        DrawCircleV(swordEnd, 6.0f, LIGHTGRAY);
    } else {
        // Draw sheathed sword
        Vector2 swordEnd = {
            ownerPos.x + aimDir.x * 25.0f,
            ownerPos.y + aimDir.y * 25.0f
        };
        DrawLineEx(ownerPos, swordEnd, 3.0f, GRAY);
    }
}
