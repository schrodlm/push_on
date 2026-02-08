#pragma once
#include "Entity.h"
#include <vector>
#include "raylib.h"

/**
 * SwordSwing - Temporary entity representing an active sword swing arc
 * Handles its own animation, drawing, and collision detection
 */
class SwordSwing : public Entity {
public:
    enum class SwingType {
        Horizontal,   // Left-right or right-left
        Overhead      // Top-down slam
    };

    struct TrailPoint {
        Vector2 position;
        float lifetime;
    };

    SwordSwing(Entity* owner, Vector2 position, float damage, float range,
               float duration, float startAngle, float endAngle, SwingType type,
               Color swingColor = WHITE);

    void Update(float deltaTime) override;
    void Draw() const override;
    void OnCollision(Entity* other) override;

    float GetDamage() const override { return m_damage; }

    bool IsInHitstop() const { return m_hitstopTimer > 0.0f; }

private:
    float m_damage;
    float m_range;
    float m_duration;
    float m_lifetime;
    float m_startAngle;  // In degrees
    float m_endAngle;    // In degrees
    SwingType m_type;
    Color m_color;

    // Visual effects
    std::vector<TrailPoint> m_trailPoints;
    float m_trailSpawnTimer;

    // Impact effects
    float m_hitstopTimer;
    bool m_hasHit;

    // Helper methods
    float GetProgress() const;
    float ApplyEasing(float t) const;
    float GetCurrentAngle() const;
    void UpdateTrail(float deltaTime);
    void TriggerImpact();
};
