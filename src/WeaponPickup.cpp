#include "WeaponPickup.h"
#include "Player.h"
#include "EntityManager.h"
#include "Logger.h"
#include <cmath>

WeaponPickup::WeaponPickup(Vector2 position, std::unique_ptr<Weapon> weapon)
    : Entity(position, 15.0f, LAYER_PICKUP, LAYER_ALL_PLAYERS, nullptr),
      m_weapon(std::move(weapon)),
      m_bobTime(0.0f),
      m_nearbyPlayer(nullptr) {
}

void WeaponPickup::Update(float deltaTime) {
    m_bobTime += deltaTime * 2.0f;  // Bob animation speed

    // Check if player presses E to pick up weapon
    if (m_nearbyPlayer && IsKeyPressed(KEY_E)) {
        PickupWeapon(m_nearbyPlayer);
    }

    // Reset nearby player each frame (will be set by OnCollision if still near)
    m_nearbyPlayer = nullptr;
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

    // Draw "Press E" prompt if player is nearby
    if (m_nearbyPlayer) {
        const char* prompt = "Press E";
        int promptWidth = MeasureText(prompt, 12);
        DrawText(prompt, drawPos.x - promptWidth / 2, drawPos.y - 30, 12, YELLOW);
    }
}

void WeaponPickup::OnCollision(Entity* other) {
    if (!m_weapon) return;

    // Track nearby player for 'E' key check
    if (auto* player = dynamic_cast<Player*>(other)) {
        m_nearbyPlayer = player;
    }
}

void WeaponPickup::PickupWeapon(Player* player) {
    if (!m_weapon || !player) return;

    Logger::Info("Player picked up weapon: ", m_weapon->GetName());

    // Drop current weapon if player has one
    if (player->HasWeapon()) {
        auto droppedWeapon = player->DropWeapon();
        Logger::Info("Dropped old weapon: ", droppedWeapon->GetName());

        // Spawn dropped weapon at player position
        auto newPickup = std::make_unique<WeaponPickup>(
            player->GetPosition(),
            std::move(droppedWeapon)
        );
        EntityManager::getInstance().queueEntity(std::move(newPickup));
    }

    // Equip new weapon
    player->EquipWeapon(std::move(m_weapon));
    Kill();
}

std::unique_ptr<Weapon> WeaponPickup::TakeWeapon() {
    return std::move(m_weapon);
}
