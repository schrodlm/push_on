#pragma once
#include "Entity.h"

/**
 * Gun bullet projectile
 * Fast, small, straightforward damage
 */
class GunBullet : public Entity {
public:
    GunBullet(Vector2 position, Vector2 velocity, float damage, Entity* owner);

    void Update(float deltaTime) override;
    void Draw() const override;
    void OnCollision(Entity* other) override;

    float GetDamage() const { return m_damage; }

private:
    Vector2 m_velocity;
    float m_damage;
};
