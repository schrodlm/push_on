#pragma once
#include "Entity.h"
#include <vector>
#include "raylib.h"

/**
 * SwordSlam - Overhead slam attack that crashes down vertically
 * Different from horizontal swings - this is a ground pound
 */
class SwordSlam : public Entity {
public:
    struct TrailPoint {
        Vector2 position;
        float lifetime;
    };

    SwordSlam(Entity* owner, Vector2 position, Vector2 direction, float damage, float range, float duration);

    void Update(float deltaTime) override;
    void Draw() const override;
    void OnCollision(Entity* other) override;

    float GetDamage() const override { return m_damage; }

private:
    float m_damage;
    float m_range;
    float m_duration;
    float m_lifetime;

    Vector2 m_direction;     // Direction of the slam (normalized)
    Vector2 m_windupOffset;  // Where the sword is raised during windup
    Vector2 m_impactPoint;   // Where the sword will crash

    // Visual effects
    std::vector<TrailPoint> m_trailPoints;
    float m_trailSpawnTimer;
    bool m_hasImpacted;

    // Helper methods
    float GetProgress() const;
    Vector2 GetCurrentSwordPosition() const;
    void UpdateTrail(float deltaTime);
};
