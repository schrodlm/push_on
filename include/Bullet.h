#pragma once
#include "Entity.h"

class Bullet : public Entity
{
public:
    Bullet(Vector2 position, Vector2 velocity, float damage)
        : Entity(position, 5.0f), m_velocity(velocity), m_damage(damage) {}

    void Update(float deltaTime) override
    {
        m_position.x += m_velocity.x * deltaTime;
        m_position.y += m_velocity.y * deltaTime;

        // Kill bullet if it goes off-screen
        if (m_position.x < 0 || m_position.x > 1280 ||
            m_position.y < 0 || m_position.y > 720)
        {
            Kill();
        }
    }

    void Draw() const override
    {
        if (m_alive)
        {
            DrawCircleV(m_position, m_radius, YELLOW);
        }
    }

    float GetDamage() const { return m_damage; }

private:
    Vector2 m_velocity;
    float m_damage;
};
