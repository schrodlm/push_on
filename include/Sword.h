#pragma once
#include "Weapon.h"

/**
 * Sword - Melee weapon with swing attack
 */
class Sword : public Weapon {
public:
    Sword();

    void Fire(Entity* owner, Vector2 target) override;
    void Update(float deltaTime) override;
    void Draw(Vector2 ownerPos, Vector2 aimDir) const override;

private:
    float m_damage;
    float m_range;
    float m_swingDuration;
    float m_swingTimer;
    bool m_isSwinging;
    Vector2 m_swingDirection;
};
