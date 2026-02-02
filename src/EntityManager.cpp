
#include "EntityManager.h"
#include "Player.h"
#include "Enemy.h"
#include <memory>
#include <algorithm>
#include <limits>
#include <cmath>

EntityManager& EntityManager::getInstance() {
    static EntityManager manager;
    return manager;
}

void EntityManager::queueEntity(std::unique_ptr<Entity> entity) {
    if (!entity) return;

    m_waiting_queue.push(std::move(entity));
}

void EntityManager::updateEntities(float deltaTime) {
    for(auto& entity : entities) {
        if (entity && entity->IsAlive()) {
            entity->Update(deltaTime);
        }
    }
}

void EntityManager::drawEntities() const {
    for(const auto& entity : entities) {
        if (entity && entity->IsAlive()) {
            entity->Draw();
        }
    }
}

void EntityManager::checkCollisions() {
    // Broad phase: Populate spatial hash with all alive entities
    m_spatialHash.Clear();
    for (auto& entity : entities) {
        if (entity && entity->IsAlive()) {
            m_spatialHash.Insert(entity.get());
        }
    }

    // Narrow phase: Check collisions for each entity
    for (auto& entity : entities) {
        if (!entity || !entity->IsAlive()) continue;

        // Query nearby entities using spatial hash
        auto nearby = m_spatialHash.QueryRadius(
            entity->GetPosition(),
            entity->GetRadius() * 2.0f  // Search radius
        );

        // Check collisions with nearby entities
        for (Entity* other : nearby) {
            // Skip self-collision and dead entities
            if (entity.get() == other || !other->IsAlive()) continue;

            // Check layer/mask filtering (fast bitwise operation)
            if (!entity->ShouldCollideWith(*other)) continue;

            // Actual collision detection (narrow phase)
            if (entity->CollidesWith(*other)) {
                // Notify both entities of the collision
                entity->OnCollision(other);
                // Note: We don't call other->OnCollision(entity.get()) here
                // because it will be handled when 'other' is processed in the outer loop
            }
        }
    }
}

void EntityManager::deleteDeadEntities() {
    // Remove dead entities from main vector
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
            [](const std::unique_ptr<Entity>& entity) {
                return !entity || !entity->IsAlive();
            }),
        entities.end()
    );

    // Clean up cached pointers
    m_players.erase(
        std::remove_if(m_players.begin(), m_players.end(),
            [](Player* player) {
                return !player || !player->IsAlive();
            }),
        m_players.end()
    );

    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [](Enemy* enemy) {
                return !enemy || !enemy->IsAlive();
            }),
        m_enemies.end()
    );
}

void EntityManager::addWaitingEntities() {
    while(!m_waiting_queue.empty()) {
        std::unique_ptr<Entity> entity = std::move(m_waiting_queue.front());
        m_waiting_queue.pop();

        // Cache typed pointers for fast, type-safe queries
        Entity* rawPtr = entity.get();

        if (auto* player = dynamic_cast<Player*>(rawPtr)) {
            m_players.push_back(player);
        }
        else if (auto* enemy = dynamic_cast<Enemy*>(rawPtr)) {
            m_enemies.push_back(enemy);
        }

        entities.push_back(std::move(entity));
    }
};

Player* EntityManager::getClosestPlayer(Vector2 position) const {
    Player* closest = nullptr;
    float minDistSq = std::numeric_limits<float>::max();

    for (Player* player : m_players) {
        if (!player || !player->IsAlive()) continue;

        Vector2 playerPos = player->GetPosition();
        float dx = playerPos.x - position.x;
        float dy = playerPos.y - position.y;
        float distSq = dx * dx + dy * dy;

        if (distSq < minDistSq) {
            minDistSq = distSq;
            closest = player;
        }
    }

    return closest;
}

Player* EntityManager::getPlayer(int playerNumber) const {
    uint32_t targetLayer = 1 << playerNumber;  // LAYER_PLAYER_1, LAYER_PLAYER_2, etc.

    for (Player* player : m_players) {
        if (player && player->IsAlive() &&
            (player->GetCollisionLayer() & targetLayer)) {
            return player;
        }
    }

    return nullptr;
}

template <typename Function>
void EntityManager::applyOnEntities(Function function) {
    for(auto && entity : entities) {
        function(entity);
    }
}   