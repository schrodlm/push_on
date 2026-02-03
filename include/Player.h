#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
    // playerNumber: 0-3 for players 1-4
    Player(Vector2 position, int playerNumber = 0);

    void Update(float deltaTime) override;
    void Draw() const override;
    void OnCollision(Entity* other) override;

    void HandleInput(float deltaTime);
    void Shoot(Vector2 target);

    float GetHealth() const { return m_health; }
    void TakeDamage(float damage);
    int GetPlayerNumber() const { return m_playerNumber; }

private:
    int m_playerNumber;
    float m_speed;
    float m_health;
    float m_maxHealth;
};
