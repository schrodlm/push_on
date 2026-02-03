#include "Gun.h"
#include "Player.h"
#include "GunBullet.h"
#include "EntityManager.h"
#include "Logger.h"
#include <cmath>

Gun::Gun()
    : Weapon("Gun", 0.15f),  // 150ms cooldown
      m_damage(25.0f),
      m_bulletSpeed(800.0f) {
}

void Gun::Fire(Player* owner, Vector2 target) {
    Logger::Debug("Gun::Fire");

    Vector2 ownerPos = owner->GetPosition();

    // Calculate direction
    Vector2 direction = {
        target.x - ownerPos.x,
        target.y - ownerPos.y
    };

    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude > 0.0f) {
        direction.x /= magnitude;
        direction.y /= magnitude;
    }

    // Calculate velocity
    Vector2 velocity = {
        direction.x * m_bulletSpeed,
        direction.y * m_bulletSpeed
    };

    // Spawn bullet slightly ahead
    Vector2 spawnPos = {
        ownerPos.x + direction.x * 25.0f,  // Spawn 25 pixels ahead
        ownerPos.y + direction.y * 25.0f
    };

    // Create bullet
    auto bullet = std::make_unique<GunBullet>(
        spawnPos,
        velocity,
        m_damage,
        owner
    );

    EntityManager::getInstance().queueEntity(std::move(bullet));

    // Reset cooldown
    m_cooldownTimer = m_cooldown;
}

void Gun::Draw(Vector2 playerPos, Vector2 aimDirection) const {
    // Draw a simple line representing the gun
    Vector2 gunEnd = {
        playerPos.x + aimDirection.x * 20.0f,
        playerPos.y + aimDirection.y * 20.0f
    };

    DrawLineEx(playerPos, gunEnd, 3.0f, DARKGRAY);
}
