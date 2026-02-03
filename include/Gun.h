#pragma once
#include "Weapon.h"

/**
 * Gun weapon 
 */
class Gun : public Weapon {
public:
    Gun();

    void Fire(Player* owner, Vector2 target) override;
    void Draw(Vector2 playerPos, Vector2 aimDirection) const override;

private:
    float m_damage;
    float m_bulletSpeed;
};
