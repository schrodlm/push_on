#include "Weapon.h"

void Weapon::Update(Entity* owner, float deltaTime) {
    if (m_cooldownTimer > 0.0f) {
        m_cooldownTimer -= deltaTime;
    }
}
