#pragma once

class AnimationMove : public Component
{
public:
    AnimationMove(const string& name = "AnimationMove") : Component(name), currentAnimation(UnitAnimationType::IDLE) {} 
    virtual ~AnimationMove() override = default;

public:
    virtual void Update() override;
    virtual void Render() override;

public:
    UnitAnimationType GetState() const { return currentAnimation; }
    void SetPlayerDead(const bool isDead) { this->isDead = isDead; }

private:
    // 현재 애니메이션 상태를 저장합니다.
    UnitAnimationType currentAnimation = UnitAnimationType::IDLE;

    bool isDead = false;

    // 현재 상태가 왼쪽을 향하고 있는지 판단하는 함수
    bool IsFacingLeft(UnitAnimationType anim) const
    {
        switch (anim)
        {
        case UnitAnimationType::RUN_L:
        case UnitAnimationType::IDLE_L:
        case UnitAnimationType::JUMP_L:
        case UnitAnimationType::JUMP_BETWEEN_L:
        case UnitAnimationType::JUMP_FALL_L:
        case UnitAnimationType::CROUCH_L:
        case UnitAnimationType::CROUCH_WALK_L:
        case UnitAnimationType::DIE_L:
            return true;
        default:
            return false;
        }
    }
};

