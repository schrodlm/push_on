#pragma once
#include "raylib.h"
#include "CollisionSystem.h"
#include <cstdint>
#include <memory>

// Forward declarations
class Weapon;

class Entity
{
public:
    Entity(Vector2 position, float radius,
           uint32_t collisionLayer, uint32_t collisionMask,
           Entity* owner = nullptr);

    virtual ~Entity();

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() const = 0;

    // Override in derived classes to handle collision responses
    virtual void OnCollision(Entity* other) {}

    // Getters
    Vector2 GetPosition() const { return m_position; }
    float GetRadius() const { return m_radius; }
    bool IsAlive() const { return m_alive; }
    uint32_t GetCollisionLayer() const { return m_collisionLayer; }
    uint32_t GetCollisionMask() const { return m_collisionMask; }
    Entity* GetOwner() const { return m_owner; }

    void Kill() { m_alive = false; }
    void SetOwner(Entity* owner) { m_owner = owner; }

    // Weapon management (optional component)
    void EquipWeapon(std::unique_ptr<Weapon> weapon);
    std::unique_ptr<Weapon> DropWeapon();
    Weapon* GetWeapon() const { return m_weapon.get(); }
    bool HasWeapon() const { return m_weapon != nullptr; }
    virtual void SetTarget(Vector2 target) {}  // For AI targeting

    /**
     * Check if this entity should collide with another based on layer/mask filtering.
     * Both entities must want to collide for collision to occur.
     * @param other The other entity to check against
     * @return true if layers/masks allow collision
     */
    bool ShouldCollideWith(const Entity& other) const
    {
        return (m_collisionMask & other.m_collisionLayer) != 0 &&
               (other.m_collisionMask & m_collisionLayer) != 0;
    }

    /**
     * Simple circle collision detection (narrow phase).
     * @param other The other entity to check collision with
     * @return true if entities are overlapping
     */
    bool CollidesWith(const Entity& other) const
    {
        float dx = m_position.x - other.m_position.x;
        float dy = m_position.y - other.m_position.y;
        float distanceSquared = dx * dx + dy * dy;
        float radiusSum = m_radius + other.m_radius;
        return distanceSquared < (radiusSum * radiusSum);
    }

protected:
    Vector2 m_position;
    float m_radius;
    bool m_alive;
    uint32_t m_collisionLayer;  // What layer(s) this entity is on
    uint32_t m_collisionMask;   // What layer(s) this entity collides with
    Entity* m_owner;            // Entity that created/owns this (e.g., who shot the bullet)

    // Optional weapon component (nullptr for entities that don't use weapons)
    std::unique_ptr<Weapon> m_weapon;
};
