#pragma once
#include "Entity.h"
#include "Bullet.h"
#include <vector>
#include <memory>

class Player : public Entity
{
public:
    Player(Vector2 position);

    void Update(float deltaTime) override;
    void Draw() const override;

    void HandleInput(float deltaTime);
    void Shoot(Vector2 target);

    std::vector<std::unique_ptr<Bullet>>& GetBullets() { return m_bullets; }

    float GetHealth() const { return m_health; }
    void TakeDamage(float damage);

private:
    float m_speed;
    float m_health;
    float m_maxHealth;
    float m_shootCooldown;
    float m_shootCooldownTimer;

    std::vector<std::unique_ptr<Bullet>> m_bullets;
};
