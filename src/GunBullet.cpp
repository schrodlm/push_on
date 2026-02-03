#include "GunBullet.h"
#include "Enemy.h"
#include "Logger.h"

GunBullet::GunBullet(Vector2 position, Vector2 velocity, float damage, Entity* owner)
    : Entity(position, 5.0f, LAYER_PLAYER_ATTACK, LAYER_ENEMY | LAYER_ENEMY_ATTACK, owner),
      m_velocity(velocity), m_damage(damage) {
}

void GunBullet::Update(float deltaTime) {
    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;

    // Kill bullet if off-screen
    if (m_position.x < 0 || m_position.x > 1280 ||
        m_position.y < 0 || m_position.y > 720) {
        Kill();
    }
}

void GunBullet::Draw() const {
    if (m_alive) {
        DrawCircleV(m_position, m_radius, YELLOW);
    }
}

void GunBullet::OnCollision(Entity* other) {
    if (other == m_owner) return;

    if (other->GetCollisionLayer() & LAYER_ENEMY) {
        if (auto* enemy = dynamic_cast<Enemy*>(other)) {
            Logger::Debug("GunBullet hit enemy for ", m_damage, " damage");
            enemy->TakeDamage(m_damage);
        }
    }

    Kill();
}
