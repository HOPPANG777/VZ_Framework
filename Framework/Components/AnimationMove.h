#pragma once

class AnimationMove : public Component
{
public:
    AnimationMove(const string& name = "AnimationMove");
    virtual ~AnimationMove() override = default;

    virtual void Update() override;
    virtual void Render() override;

    UnitAnimationType GetCurrentAnimation() const { return currentAnimation; }
    void SetPlayerDead(bool dead) { isDead = dead; }

private:
    // 현재 애니메이션 상태 (예: Idle_R, Run_R, Jump_R 등)
    UnitAnimationType currentAnimation;
    bool isDead;

    // 현재 상태가 왼쪽을 향하는지 판단 (애니메이션 이름 기준)
    bool IsFacingLeft(UnitAnimationType anim) const;
};

