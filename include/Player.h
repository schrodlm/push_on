#pragma once
#include "Entity.h"
#include "Weapon.h"
#include <memory>
#include <vector>

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

    // Weapon management
    void EquipWeapon(std::unique_ptr<Weapon> weapon);
    void AddToInventory(std::unique_ptr<Weapon> weapon);
    void SwitchWeapon(int index);

    float GetHealth() const { return m_health; }
    void TakeDamage(float damage);
    int GetPlayerNumber() const { return m_playerNumber; }

private:
    int m_playerNumber;
    float m_speed;
    float m_health;
    float m_maxHealth;

    // Weapon system
    std::unique_ptr<Weapon> m_equippedWeapon;
    std::vector<std::unique_ptr<Weapon>> m_inventory;
    int m_currentWeaponIndex;
};
