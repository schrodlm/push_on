#pragma once
#include "Weapon.h"

/**
 * Sword - Melee weapon with 3-hit combo system
 */
class Sword : public Weapon {
public:
    Sword();

    void Fire(Entity* owner, Vector2 target) override;
    void Update(Entity* owner, float deltaTime) override;
    void Draw(Vector2 ownerPos, Vector2 aimDir) const override;

private:
    enum class ComboStage {
        None,
        Swing1_LeftRight,   // Horizontal left to right
        Swing2_RightLeft,   // Horizontal right to left
        Swing3_Overhead     // Vertical overhead slam
    };

    struct SwingConfig {
        float duration;
        float startAngleOffset;  // Relative to aim direction
        float endAngleOffset;
        float damage;
        float range;
    };


    // Combo state
    ComboStage m_currentCombo;
    float m_comboResetTimer;
    float m_comboWindow;

    // Swing state
    float m_swingTimer;
    bool m_isSwinging;
    Vector2 m_swingDirection;
    SwingConfig m_currentSwing;

    // Swing configurations
    SwingConfig GetSwingConfig(ComboStage stage) const;
};
