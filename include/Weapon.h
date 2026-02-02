#pragma once
#include "Entity.h"
#include <memory>
#include <raylib.h>

class Weapon : public Entity {

public:
    void Draw(float deltaTime);
    void Fire(const Vector2 target);


private:
    float m_fireCooldown;
    float m_fireCooldownTimer;
    float m_damage;
    float m_speed;
    
    std::unique_ptr<Entity> m_owner;

    Vector2 get_position();
    uint32_t m_bullets;
};