#pragma once
#include "Entity.h"
#include "Weapon.h"
#include <memory>

/**
 * WeaponPickup - Entity for weapons in the world
 */
class WeaponPickup : public Entity {
public:
    WeaponPickup(Vector2 position, std::unique_ptr<Weapon> weapon);

    void Update(float deltaTime) override;
    void Draw() const override;
    void OnCollision(Entity* other) override;

    std::unique_ptr<Weapon> TakeWeapon();

private:
    void PickupWeapon(Player* player);

    std::unique_ptr<Weapon> m_weapon;
    float m_bobTime;  // For floating animation
    Player* m_nearbyPlayer;  // Track nearby player for 'E' prompt
};
