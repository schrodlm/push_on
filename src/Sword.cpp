#include "Sword.h"
#include "EntityManager.h"
#include "SwordSwing.h"
#include "SwordSlam.h"
#include "Logger.h"
#include <cmath>
#include <raylib.h>

Sword::Sword()
    : Weapon("Sword", 0.1f),  // Short cooldown for combo responsiveness
      m_currentCombo(ComboStage::None),
      m_comboResetTimer(0.0f),
      m_comboWindow(1.0f),  // 1 second to continue combo
      m_swingTimer(0.0f),
      m_isSwinging(false),
      m_swingDirection({0.0f, 0.0f}) {
}

Sword::SwingConfig Sword::GetSwingConfig(ComboStage stage) const {
    SwingConfig config;

    switch (stage) {
        case ComboStage::Swing1_LeftRight:
            config.duration = 0.35f;
            config.startAngleOffset = -90.0f;  // Left
            config.endAngleOffset = 90.0f;     // Right
            config.damage = 30.0f;
            config.range = 65.0f;
            break;

        case ComboStage::Swing2_RightLeft:
            config.duration = 0.3f;
            config.startAngleOffset = 90.0f;   // Right
            config.endAngleOffset = -90.0f;    // Left
            config.damage = 35.0f;
            config.range = 65.0f;
            break;

        case ComboStage::Swing3_Overhead:
            config.duration = 0.5f;
            config.startAngleOffset = -120.0f; // Up and back
            config.endAngleOffset = 60.0f;     // Down in front
            config.damage = 60.0f;             // Heavy finisher
            config.range = 70.0f;
            break;

        default:
            config.duration = 0.35f;
            config.startAngleOffset = -90.0f;
            config.endAngleOffset = 90.0f;
            config.damage = 30.0f;
            config.range = 65.0f;
            break;
    }

    return config;
}

void Sword::Fire(Entity* owner, Vector2 target) {
    if (!CanFire() || !owner || m_isSwinging) return;

    // Advance combo or reset
    if (m_comboResetTimer > 0.0f) {
        // Continue combo
        switch (m_currentCombo) {
            case ComboStage::None:
            case ComboStage::Swing3_Overhead:
                m_currentCombo = ComboStage::Swing1_LeftRight;
                break;
            case ComboStage::Swing1_LeftRight:
                m_currentCombo = ComboStage::Swing2_RightLeft;
                break;
            case ComboStage::Swing2_RightLeft:
                m_currentCombo = ComboStage::Swing3_Overhead;
                break;
        }
    } else {
        // Start fresh combo
        m_currentCombo = ComboStage::Swing1_LeftRight;
    }

    Logger::Debug("Sword::Fire - Combo stage: ", static_cast<int>(m_currentCombo));

    // Setup swing
    m_isSwinging = true;
    m_currentSwing = GetSwingConfig(m_currentCombo);
    m_swingTimer = m_currentSwing.duration;
    m_cooldownTimer = m_cooldown;
    m_comboResetTimer = m_comboWindow;

    // Calculate swing direction
    Vector2 ownerPos = owner->GetPosition();
    m_swingDirection = {
        target.x - ownerPos.x,
        target.y - ownerPos.y
    };
    float magnitude = std::sqrt(m_swingDirection.x * m_swingDirection.x +
                                m_swingDirection.y * m_swingDirection.y);
    if (magnitude > 0.0f) {
        m_swingDirection.x /= magnitude;
        m_swingDirection.y /= magnitude;
    }

    // Third combo is a slam, not a swing
    if (m_currentCombo == ComboStage::Swing3_Overhead) {
        // Create overhead slam attack in the direction player is aiming
        auto slam = std::make_unique<SwordSlam>(owner, ownerPos, m_swingDirection,
                                                 m_currentSwing.damage, m_currentSwing.range,
                                                 m_currentSwing.duration);
        EntityManager::getInstance().queueEntity(std::move(slam));
    } else {
        // Create horizontal swing for first two combos
        float baseAngle = std::atan2(m_swingDirection.y, m_swingDirection.x) * RAD2DEG;
        float startAngle = baseAngle + m_currentSwing.startAngleOffset;
        float endAngle = baseAngle + m_currentSwing.endAngleOffset;

        Color swingColor = (m_currentCombo == ComboStage::Swing2_RightLeft) ? SKYBLUE : WHITE;

        auto swing = std::make_unique<SwordSwing>(owner, ownerPos, m_currentSwing.damage,
                                                    m_currentSwing.range, m_currentSwing.duration,
                                                    startAngle, endAngle, swingColor);
        EntityManager::getInstance().queueEntity(std::move(swing));
    }
}

void Sword::Update(Entity* owner, float deltaTime) {
    // Call base class to update cooldown
    Weapon::Update(owner, deltaTime);

    // Update combo reset timer
    if (m_comboResetTimer > 0.0f) {
        m_comboResetTimer -= deltaTime;
        if (m_comboResetTimer <= 0.0f) {
            m_currentCombo = ComboStage::None;
        }
    }

    // Update swing timer
    if (m_swingTimer > 0.0f) {
        m_swingTimer -= deltaTime;
        if (m_swingTimer <= 0.0f) {
            m_isSwinging = false;
        }
    }
}

void Sword::Draw(Vector2 ownerPos, Vector2 aimDir) const {
    // Only draw sheathed sword when not swinging
    // Active swings are drawn by SwordSwing entities
    if (!m_isSwinging) {
        Vector2 swordEnd = {
            ownerPos.x + aimDir.x * 25.0f,
            ownerPos.y + aimDir.y * 25.0f
        };

        Color swordColor = GRAY;

        // Show combo readiness with pulsing yellow glow
        if (m_comboResetTimer > 0.0f) {
            float pulseAlpha = 0.5f + 0.5f * std::sin(GetTime() * 10.0f);
            swordColor = ColorAlpha(YELLOW, pulseAlpha);
        }

        DrawLineEx(ownerPos, swordEnd, 3.0f, swordColor);
    }
}
