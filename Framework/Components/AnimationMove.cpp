#include "Framework.h"
#include "AnimationMove.h"

AnimationMove::AnimationMove(const string& name)
    : Component(name), currentAnimation(UnitAnimationType::IDLE_R), isDead(false)
{
}

bool AnimationMove::IsFacingLeft(UnitAnimationType anim) const
{
    // 왼쪽 상태로 간주할 애니메이션 (상태명에 _L 포함)
    switch (anim)
    {
    case UnitAnimationType::IDLE_L:
    case UnitAnimationType::RUN_L:
    case UnitAnimationType::JUMP_L:
    case UnitAnimationType::JUMP_BETWEEN_L:
    case UnitAnimationType::JUMP_FALL_L:
    case UnitAnimationType::AXE_THROW_PREPARE_L:
    case UnitAnimationType::AXE_THROW_L:
    case UnitAnimationType::AXE_RETURN_L:
    case UnitAnimationType::CROUCH_L:
    case UnitAnimationType::CROUCH_WALK_L:
        return true;
    default:
        return false;
    }
}

void AnimationMove::Update()
{
    auto animator = owner->GetComponent<Animator>("Animator");
    if (!animator) return;

    // (A) 사망 상태 체크: 사망이면 바로 죽음 애니메이션을 재생
    if (isDead)
    {
        UnitAnimationType deathState = IsFacingLeft(currentAnimation) ? UnitAnimationType::DIE_L : UnitAnimationType::DIE_R;
        if (currentAnimation != deathState)
        {
            currentAnimation = deathState;
            animator->SetCurrentAnimClip(deathState == UnitAnimationType::DIE_L ? L"DieL" : L"DieR");
        }
        animator->Update();
        return;
    }

    // (B) 살아있는 경우: 입력에 따라 새로운 애니메이션 상태 결정
    UnitAnimationType newState = currentAnimation;

    // 예시로 단순하게 처리 (실제 게임에서는 PlayerMove의 물리 상태나 입력을 참조)
    if (INPUT->KeyDown(VK_SPACE))
    {
        newState = IsFacingLeft(currentAnimation) ? UnitAnimationType::JUMP_L : UnitAnimationType::JUMP_R;
    }
    else if (INPUT->KeyPressed('A'))
    {
        newState = UnitAnimationType::RUN_L;
    }
    else if (INPUT->KeyPressed('D'))
    {
        newState = UnitAnimationType::RUN_R;
    }
    else
    {
        newState = IsFacingLeft(currentAnimation) ? UnitAnimationType::IDLE_L : UnitAnimationType::IDLE_R;
    }

    // 도끼 관련 입력 처리
    if (INPUT->MouseDown(2))
    {
        newState = IsFacingLeft(currentAnimation) ? UnitAnimationType::AXE_THROW_PREPARE_L : UnitAnimationType::AXE_THROW_PREPARE_R;
    }
    if (INPUT->MousePressed(2) && INPUT->MouseDown(1))
    {
        newState = IsFacingLeft(currentAnimation) ? UnitAnimationType::AXE_THROW_L : UnitAnimationType::AXE_THROW_R;
    }
    if (INPUT->KeyDown('E'))
    {
        newState = IsFacingLeft(currentAnimation) ? UnitAnimationType::AXE_RETURN_L : UnitAnimationType::AXE_RETURN_R;
    }

    // 상태 변화가 있으면 애니메이션 클립 전환
    if (newState != currentAnimation)
    {
        currentAnimation = newState;
        switch (currentAnimation)
        {
        case UnitAnimationType::IDLE_R:    animator->SetCurrentAnimClip(L"IdleR"); break;
        case UnitAnimationType::IDLE_L:    animator->SetCurrentAnimClip(L"IdleL"); break;
        case UnitAnimationType::RUN_R:     animator->SetCurrentAnimClip(L"RunR"); break;
        case UnitAnimationType::RUN_L:     animator->SetCurrentAnimClip(L"RunL"); break;
        case UnitAnimationType::JUMP_R:    animator->SetCurrentAnimClip(L"JumpR"); break;
        case UnitAnimationType::JUMP_L:    animator->SetCurrentAnimClip(L"JumpL"); break;
        case UnitAnimationType::JUMP_BETWEEN_R: animator->SetCurrentAnimClip(L"JumpBetweenR"); break;
        case UnitAnimationType::JUMP_BETWEEN_L: animator->SetCurrentAnimClip(L"JumpBetweenL"); break;
        case UnitAnimationType::JUMP_FALL_R: animator->SetCurrentAnimClip(L"JumpFallR"); break;
        case UnitAnimationType::JUMP_FALL_L: animator->SetCurrentAnimClip(L"JumpFallL"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_R: animator->SetCurrentAnimClip(L"AxeThrowPrepareR"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_L: animator->SetCurrentAnimClip(L"AxeThrowPrepareL"); break;
        case UnitAnimationType::AXE_THROW_R: animator->SetCurrentAnimClip(L"AxeThrowR"); break;
        case UnitAnimationType::AXE_THROW_L: animator->SetCurrentAnimClip(L"AxeThrowL"); break;
        case UnitAnimationType::AXE_RETURN_R: animator->SetCurrentAnimClip(L"AxeReturnR"); break;
        case UnitAnimationType::AXE_RETURN_L: animator->SetCurrentAnimClip(L"AxeReturnL"); break;
        default: break;
        }
    }

    animator->Update();
}

void AnimationMove::Render()
{
    // 애니메이션 컴포넌트는 별도의 렌더링 없이 Animator를 업데이트함.
}
