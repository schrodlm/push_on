#pragma once
#include "raylib.h"
#include <string>

// Forward declarations
class Player;

/**
 * Base weapon class
 * Weapons are behavior classes that create projectile entities when fired.
 * When equipped, they are part of the player, not separate world objects.
 */
class Weapon {
public:
    virtual ~Weapon() = default;

    /**
     * Fire the weapon, creating projectile entities
     * @param owner The player firing the weapon
     * @param target The target position to shoot at
     */
    virtual void Fire(Player* owner, Vector2 target) = 0;

    /**
     * Update weapon state (cooldowns, animations, etc)
     */
    virtual void Update(float deltaTime);

    /**
     * Draw weapon at player position
     */
    virtual void Draw(Vector2 playerPos, Vector2 aimDirection) const = 0;

    /**
     * Check if weapon can fire (cooldown ready)
     */
    bool CanFire() const { return m_cooldownTimer <= 0.0f; }

    // Getters
    float GetCooldown() const { return m_cooldown; }
    const std::string& GetName() const { return m_name; }

protected:
    Weapon(const std::string& name, float cooldown)
        : m_name(name), m_cooldown(cooldown), m_cooldownTimer(0.0f) {}

    std::string m_name;
    float m_cooldown;
    float m_cooldownTimer;
};
