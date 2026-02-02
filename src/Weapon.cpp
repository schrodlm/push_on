#include "Weapon.h"
#include <cassert>
#include <cmath>
#include "EntityManager.h"
#include "Bullet.h"
#include "CollisionSystem.h"

void Weapon::Fire(const Vector2 target) {

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

    // Create bullet with collision layers
    // TODO: Determine proper layer based on weapon owner
    EntityManager::getInstance().queueEntity(std::make_unique<Bullet>(
        spawnPos, velocity, 25.0f,
        LAYER_PLAYER_ATTACK,           // Default to player attack
        LAYER_ENEMY,                    // Hit enemies
        m_owner.get()                   // Owner of the weapon
    ));
}
