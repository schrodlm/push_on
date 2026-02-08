#include "SwordSwing.h"
#include <cmath>

SwordSwing::SwordSwing(Entity* owner, Vector2 position, float damage, float range,
                       float duration, float startAngle, float endAngle, SwingType type,
                       Color swingColor)
    : Entity(position, range,  // Use range as radius for collision
             owner->GetCollisionLayer() == LAYER_ENEMY ? LAYER_ENEMY_ATTACK : LAYER_PLAYER_ATTACK,
             owner->GetCollisionMask(),
             owner)
    , m_damage(damage)
    , m_range(range)
    , m_duration(duration)
    , m_lifetime(0.0f)
    , m_startAngle(startAngle)
    , m_endAngle(endAngle)
    , m_type(type)
    , m_color(swingColor)
    , m_trailSpawnTimer(0.0f) {
}

float SwordSwing::GetProgress() const {
    if (m_duration <= 0.0f) return 1.0f;
    return m_lifetime / m_duration;
}

float SwordSwing::ApplyEasing(float t) const {
    if (m_type == SwingType::Overhead) {
        // Overhead: Slow windup (70% of time), then FAST crash down
        if (t < 0.7f) {
            // Slow ease-in for anticipation
            float normalized = t / 0.7f;
            return normalized * normalized * 0.3f;  // Only travel 30% during windup
        } else {
            // Fast ease-out for the slam
            float normalized = (t - 0.7f) / 0.3f;
            return 0.3f + (1.0f - std::pow(1.0f - normalized, 4.0f)) * 0.7f;
        }
    } else {
        // Horizontal swings: Smooth ease-in-out cubic
        if (t < 0.5f) {
            return 4.0f * t * t * t;
        } else {
            float f = 2.0f * t - 2.0f;
            return 0.5f * f * f * f + 1.0f;
        }
    }
}

float SwordSwing::GetCurrentAngle() const {
    float progress = ApplyEasing(GetProgress());
    return m_startAngle + (m_endAngle - m_startAngle) * progress;
}

void SwordSwing::UpdateTrail(float deltaTime) {
    // Add new trail points
    m_trailSpawnTimer -= deltaTime;
    if (m_trailSpawnTimer <= 0.0f) {
        float currentAngle = GetCurrentAngle();
        float angleRad = currentAngle * DEG2RAD;
        Vector2 swordTip = {
            m_position.x + std::cos(angleRad) * m_range,
            m_position.y + std::sin(angleRad) * m_range
        };
        m_trailPoints.push_back({swordTip, 0.2f});  // 200ms lifetime
        m_trailSpawnTimer = 0.02f;  // Spawn every 20ms
    }

    // Update and remove old trail points
    for (auto it = m_trailPoints.begin(); it != m_trailPoints.end();) {
        it->lifetime -= deltaTime;
        if (it->lifetime <= 0.0f) {
            it = m_trailPoints.erase(it);
        } else {
            ++it;
        }
    }
}

void SwordSwing::Update(float deltaTime) {
    m_lifetime += deltaTime;

    // Die when swing animation is complete
    if (m_lifetime >= m_duration) {
        Kill();
        return;
    }

    // Update position to follow owner if they moved
    if (m_owner && m_owner->IsAlive()) {
        m_position = m_owner->GetPosition();
    }

    // Update visual trail
    UpdateTrail(deltaTime);

    // For overhead slams, spawn shockwave at the end
    if (m_type == SwingType::Overhead && GetProgress() >= 0.95f && m_lifetime < m_duration - deltaTime) {
        // TODO: Spawn shockwave entity here when implemented
    }
}

void SwordSwing::Draw() const {
    if (!m_alive) return;

    float currentAngle = GetCurrentAngle();
    float currentAngleRad = currentAngle * DEG2RAD;

    // Calculate sword tip position
    Vector2 swordEnd = {
        m_position.x + std::cos(currentAngleRad) * m_range,
        m_position.y + std::sin(currentAngleRad) * m_range
    };

    // Draw motion trail
    for (const auto& point : m_trailPoints) {
        float alpha = point.lifetime / 0.2f;
        DrawCircleV(point.position, 3.0f, Fade(m_color, alpha * 0.6f));
    }

    // Draw arc sweep visualization (partial arc behind the blade)
    float arcStartAngle = currentAngle - 30.0f;
    float arcEndAngle = currentAngle;

    for (float a = arcStartAngle; a < arcEndAngle; a += 5.0f) {
        float alpha = 1.0f - ((arcEndAngle - a) / 30.0f);
        float rad = a * DEG2RAD;
        Vector2 arcPoint = {
            m_position.x + std::cos(rad) * m_range * 0.8f,
            m_position.y + std::sin(rad) * m_range * 0.8f
        };
        DrawCircleV(arcPoint, 2.0f, Fade(m_color, alpha * 0.3f));
    }

    // Draw the blade itself
    float bladeThickness = (m_type == SwingType::Overhead) ? 7.0f : 5.0f;

    // Draw blade with glow
    DrawLineEx(m_position, swordEnd, bladeThickness + 2.0f, Fade(m_color, 0.3f));
    DrawLineEx(m_position, swordEnd, bladeThickness, Fade(m_color, 0.9f));
    DrawCircleV(swordEnd, 8.0f, Fade(m_color, 0.7f));
}

void SwordSwing::OnCollision(Entity* other) {
    // SwordSwing entities don't respond to collisions themselves
    // The collision system will call OnCollision on the 'other' entity
    // which will handle taking damage
}
