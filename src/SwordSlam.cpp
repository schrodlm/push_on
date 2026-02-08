#include "SwordSlam.h"
#include <cmath>

SwordSlam::SwordSlam(Entity* owner, Vector2 position, Vector2 direction, float damage, float range, float duration)
    : Entity(position, range,
             owner->GetCollisionLayer() == LAYER_ENEMY ? LAYER_ENEMY_ATTACK : LAYER_PLAYER_ATTACK,
             owner->GetCollisionMask(),
             owner)
    , m_damage(damage)
    , m_range(range)
    , m_duration(duration)
    , m_lifetime(0.0f)
    , m_direction(direction)
    , m_windupOffset({direction.x * -60.0f, direction.y * -60.0f})  // Raise sword back 60 pixels
    , m_impactPoint({position.x + direction.x * range, position.y + direction.y * range})  // Impact in front
    , m_trailSpawnTimer(0.0f)
    , m_hasImpacted(false) {
}

float SwordSlam::GetProgress() const {
    if (m_duration <= 0.0f) return 1.0f;
    return m_lifetime / m_duration;
}

Vector2 SwordSlam::GetCurrentSwordPosition() const {
    float progress = GetProgress();

    if (progress < 0.6f) {
        // Windup phase (60% of time) - slowly raise sword
        float windupProgress = progress / 0.6f;
        float eased = windupProgress * windupProgress;  // Ease-in
        return {
            m_position.x + m_windupOffset.x * eased,
            m_position.y + m_windupOffset.y * eased
        };
    } else {
        // Slam phase (40% of time) - FAST crash down
        float slamProgress = (progress - 0.6f) / 0.4f;
        float eased = 1.0f - std::pow(1.0f - slamProgress, 4.0f);  // Ease-out quartic (very fast)

        Vector2 startPos = {
            m_position.x + m_windupOffset.x,
            m_position.y + m_windupOffset.y
        };

        return {
            startPos.x + (m_impactPoint.x - startPos.x) * eased,
            startPos.y + (m_impactPoint.y - startPos.y) * eased
        };
    }
}

void SwordSlam::UpdateTrail(float deltaTime) {
    m_trailSpawnTimer -= deltaTime;
    if (m_trailSpawnTimer <= 0.0f) {
        Vector2 swordPos = GetCurrentSwordPosition();
        m_trailPoints.push_back({swordPos, 0.15f});
        m_trailSpawnTimer = 0.015f;
    }

    for (auto it = m_trailPoints.begin(); it != m_trailPoints.end();) {
        it->lifetime -= deltaTime;
        if (it->lifetime <= 0.0f) {
            it = m_trailPoints.erase(it);
        } else {
            ++it;
        }
    }
}

void SwordSlam::Update(float deltaTime) {
    m_lifetime += deltaTime;

    if (m_lifetime >= m_duration) {
        Kill();
        return;
    }

    // Update position to follow owner
    if (m_owner && m_owner->IsAlive()) {
        m_position = m_owner->GetPosition();
        // Update impact point to stay in front of owner
        m_impactPoint = {
            m_position.x + m_direction.x * m_range,
            m_position.y + m_direction.y * m_range
        };
        // Update windup offset too
        m_windupOffset = {
            m_direction.x * -60.0f,
            m_direction.y * -60.0f
        };
    }

    UpdateTrail(deltaTime);

    // Check for impact at ~95% progress
    float progress = GetProgress();
    if (!m_hasImpacted && progress >= 0.95f) {
        m_hasImpacted = true;
        // TODO: Trigger screen shake and spawn shockwave
    }
}

void SwordSlam::Draw() const {
    if (!m_alive) return;

    float progress = GetProgress();
    Vector2 swordPos = GetCurrentSwordPosition();

    // Draw motion trail
    for (const auto& point : m_trailPoints) {
        float alpha = point.lifetime / 0.15f;
        float size = 6.0f * alpha;
        DrawCircleV(point.position, size, Fade(ORANGE, alpha * 0.5f));
    }

    // Windup phase - show charging indicator
    if (progress < 0.6f) {
        float windupProgress = progress / 0.6f;

        // Pulsing warning circle at impact point
        float pulseRadius = 40.0f + 20.0f * std::sin(m_lifetime * 15.0f);
        DrawCircleLines(m_impactPoint.x, m_impactPoint.y, pulseRadius, Fade(ORANGE, 0.4f * windupProgress));
        DrawCircleV(m_impactPoint, 15.0f * windupProgress, Fade(ORANGE, 0.3f));

        // Raised sword with glow
        DrawCircleV(swordPos, 25.0f, Fade(ORANGE, 0.4f + 0.3f * windupProgress));
        DrawLineEx(m_position, swordPos, 8.0f, Fade(ORANGE, 0.6f));
    } else {
        // Slam phase - motion blur
        float slamProgress = (progress - 0.6f) / 0.4f;

        // Draw multiple positions for motion blur effect
        for (int i = 0; i < 5; i++) {
            float offset = i * 15.0f;
            Vector2 blurPos = {
                swordPos.x,
                swordPos.y - offset
            };
            float alpha = 1.0f - (i / 5.0f);
            DrawLineEx(m_position, blurPos, 12.0f, Fade(ORANGE, alpha * 0.3f));
        }

        // Impact flash when hitting
        if (progress >= 0.95f) {
            float impactFlash = 1.0f - ((progress - 0.95f) / 0.05f);
            DrawCircleV(m_impactPoint, 60.0f * impactFlash, Fade(WHITE, impactFlash * 0.8f));
            DrawCircleV(m_impactPoint, 40.0f * impactFlash, Fade(ORANGE, impactFlash));
        }
    }

    // Draw the blade
    float thickness = (progress >= 0.6f) ? 10.0f : 8.0f;
    DrawLineEx(m_position, swordPos, thickness + 2.0f, Fade(ORANGE, 0.4f));
    DrawLineEx(m_position, swordPos, thickness, Fade(ORANGE, 0.9f));
    DrawCircleV(swordPos, 12.0f, Fade(ORANGE, 0.8f));
}

void SwordSlam::OnCollision(Entity* other) {
    if (other && other != m_owner && GetProgress() >= 0.9f) {
        // Only hit during the impact phase
    }
}
