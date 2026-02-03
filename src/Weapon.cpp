#include "Weapon.h"

void Weapon::Update(float deltaTime) {
    if (m_cooldownTimer > 0.0f) {
        m_cooldownTimer -= deltaTime;
    }
}
