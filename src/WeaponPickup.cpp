#include "WeaponPickup.h"
#include "Player.h"
#include "Logger.h"
#include <cmath>

WeaponPickup::WeaponPickup(Vector2 position, std::unique_ptr<Weapon> weapon)
    : Entity(position, 15.0f, LAYER_PICKUP, LAYER_ALL_PLAYERS, nullptr),
      m_weapon(std::move(weapon)),
      m_bobTime(0.0f) {
}

void WeaponPickup::Update(float deltaTime) {
    m_bobTime += deltaTime * 2.0f;  // Bob animation speed
}

void WeaponPickup::Draw() const {
    if (!m_alive || !m_weapon) return;

    // Floating animation
    float bobOffset = std::sin(m_bobTime) * 5.0f;
    Vector2 drawPos = { m_position.x, m_position.y + bobOffset };

    // Draw as a box with weapon name
    DrawCircleV(drawPos, m_radius, GOLD);
    DrawCircleLinesV(drawPos, m_radius, ORANGE);

    // Draw weapon name below
    const char* name = m_weapon->GetName().c_str();
    int textWidth = MeasureText(name, 10);
    DrawText(name, drawPos.x - textWidth / 2, drawPos.y + 20, 10, WHITE);
}

void WeaponPickup::OnCollision(Entity* other) {
    if (!m_weapon) return;

    if (auto* player = dynamic_cast<Player*>(other)) {
        Logger::Info("Player picked up weapon: ", m_weapon->GetName());
        player->AddToInventory(std::move(m_weapon));
        Kill();
    }
}

std::unique_ptr<Weapon> WeaponPickup::TakeWeapon() {
    return std::move(m_weapon);
}
