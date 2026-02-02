#pragma once
#include <queue>
#include <vector>
#include <memory>
#include "Entity.h"
#include "CollisionSystem.h"

// Forward declarations
class Player;
class Enemy;

class EntityManager {
public:
    void queueEntity(std::unique_ptr<Entity> entity);
    void deleteDeadEntities();
    void addWaitingEntities();
    void updateEntities(float deltaTime);
    void drawEntities() const;
    void checkCollisions();
    static EntityManager& getInstance();

    // Type-safe queries (no casting needed!)
    const std::vector<Player*>& getPlayers() const { return m_players; }
    const std::vector<Enemy*>& getEnemies() const { return m_enemies; }

    // Helper methods
    Player* getClosestPlayer(Vector2 position) const;
    Player* getPlayer(int playerNumber = 0) const;  // Get specific player by number

    // Get all entities (for advanced use cases)
    const std::vector<std::unique_ptr<Entity>>& getEntities() const { return entities; }

    template<typename Function>
    void applyOnEntities(Function function);

private:
    std::vector<std::unique_ptr<Entity>> entities;

    // Cached typed pointers (updated automatically)
    std::vector<Player*> m_players;
    std::vector<Enemy*> m_enemies;

    std::queue<std::unique_ptr<Entity>> m_waiting_queue;


    SpatialHash m_spatialHash;
};