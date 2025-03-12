#pragma once

class AnimationMove : public Component
{
public:
    AnimationMove(const string& name = "AnimationMove") : Component(name), currentAnimation(UnitAnimationType::IDLE) {} 
    virtual ~AnimationMove() override = default;

    virtual void Update() override;
    virtual void Render() override;

    UnitAnimationType GetCurrentAnimation() const { return currentAnimation; }

private:
    // 현재 애니메이션 상태를 저장합니다.
    UnitAnimationType currentAnimation = UnitAnimationType::IDLE;
};

