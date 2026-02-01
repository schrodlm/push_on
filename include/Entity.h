#pragma once
#include "raylib.h"

class Entity
{
public:
    Entity(Vector2 position, float radius)
        : m_position(position), m_radius(radius), m_alive(true) {}

    virtual ~Entity() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() const = 0;

    Vector2 GetPosition() const { return m_position; }
    float GetRadius() const { return m_radius; }
    bool IsAlive() const { return m_alive; }

    void Kill() { m_alive = false; }

    // Simple circle collision
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
};
