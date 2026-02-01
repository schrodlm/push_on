#pragma once
#include "Entity.h"

class Enemy : public Entity
{
public:
    Enemy(Vector2 position, float health = 50.0f);

    void Update(float deltaTime) override;
    void Draw() const override;

    void SetTarget(Vector2 target) { m_target = target; }
    void TakeDamage(float damage);

    float GetHealth() const { return m_health; }
    float GetDamage() const { return m_contactDamage; }

private:
    Vector2 m_target;
    float m_speed;
    float m_health;
    float m_maxHealth;
    float m_contactDamage;
};
