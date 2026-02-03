#include "Entity.h"
#include "Weapon.h"

Entity::Entity(Vector2 position, float radius,
               uint32_t collisionLayer, uint32_t collisionMask,
               Entity* owner)
    : m_position(position), m_radius(radius), m_alive(true),
      m_collisionLayer(collisionLayer), m_collisionMask(collisionMask),
      m_owner(owner), m_weapon(nullptr) {
}

Entity::~Entity() = default;

void Entity::EquipWeapon(std::unique_ptr<Weapon> weapon) {
    m_weapon = std::move(weapon);
}

std::unique_ptr<Weapon> Entity::DropWeapon() {
    return std::move(m_weapon);
}
