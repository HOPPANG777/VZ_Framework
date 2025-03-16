#include "Framework.h"
#include "AnimationMove.h"

void AnimationMove::Update()
{
    auto animator = owner->GetComponent<Animator>("Animator");
    if (!animator) return;

    // (A) 사망 상태 체크
    if (isDead)
    {
        bool facingLeft = IsFacingLeft(currentAnimation);
        UnitAnimationType newState = facingLeft ? UnitAnimationType::DIE_L : UnitAnimationType::DIE_R;

        if (newState != currentAnimation)
        {
            currentAnimation = newState;
            switch (currentAnimation)
            {
            case UnitAnimationType::DIE_L:
                animator->SetCurrentAnimClip(L"DieL");
                break;
            case UnitAnimationType::DIE_R:
                animator->SetCurrentAnimClip(L"DieR");
                break;
            default:
                break;
            }
        }
        animator->Update();
        return;
    }

    // (B) 살아있는 경우: 입력에 따른 상태 결정
    UnitAnimationType newState = currentAnimation;

    // 숙이기: 컨트롤 키 입력
    if (INPUT->KeyPressed(VK_CONTROL))
    {
        if (INPUT->KeyPressed('A'))
        {
            newState = UnitAnimationType::CROUCH_WALK_L;
        }
        else if (INPUT->KeyPressed('D'))
        {
            newState = UnitAnimationType::CROUCH_WALK_R;
        }
        else
        {
            newState = IsFacingLeft(currentAnimation) ? UnitAnimationType::CROUCH_L : UnitAnimationType::CROUCH_R;
        }
    }
    // 점프: 스페이스바 입력
    else if (INPUT->KeyPressed(VK_SPACE))
    {
        newState = IsFacingLeft(currentAnimation) ? UnitAnimationType::JUMP_L : UnitAnimationType::JUMP_R;
    }
    // 좌우 이동
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

    // 도끼 관련 입력 처리:
    // 여기서는 현재 바라보는 방향을 기준으로 좌우 버전으로 설정합니다.
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

    // 상태가 변경되었으면 애니메이터에 반영
    if (newState != currentAnimation)
    {
        currentAnimation = newState;
        switch (currentAnimation)
        {
        case UnitAnimationType::IDLE_R:           animator->SetCurrentAnimClip(L"IdleR");           break;
        case UnitAnimationType::IDLE_L:           animator->SetCurrentAnimClip(L"IdleL");           break;
        case UnitAnimationType::RUN_R:            animator->SetCurrentAnimClip(L"RunR");            break;
        case UnitAnimationType::RUN_L:            animator->SetCurrentAnimClip(L"RunL");            break;
        case UnitAnimationType::JUMP_R:           animator->SetCurrentAnimClip(L"JumpR");           break;
        case UnitAnimationType::JUMP_L:           animator->SetCurrentAnimClip(L"JumpL");           break;
        case UnitAnimationType::AXE_THROW_PREPARE_R:animator->SetCurrentAnimClip(L"AxeThrowPrepareR"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_L:animator->SetCurrentAnimClip(L"AxeThrowPrepareL"); break;
        case UnitAnimationType::AXE_THROW_R:        animator->SetCurrentAnimClip(L"AxeThrowR");          break;
        case UnitAnimationType::AXE_THROW_L:        animator->SetCurrentAnimClip(L"AxeThrowL");          break;
        case UnitAnimationType::AXE_RETURN_R:       animator->SetCurrentAnimClip(L"AxeReturnR");         break;
        case UnitAnimationType::AXE_RETURN_L:       animator->SetCurrentAnimClip(L"AxeReturnL");         break;
        case UnitAnimationType::CROUCH_R:           animator->SetCurrentAnimClip(L"CrouchR");           break;
        case UnitAnimationType::CROUCH_L:           animator->SetCurrentAnimClip(L"CrouchL");           break;
        case UnitAnimationType::CROUCH_WALK_R:      animator->SetCurrentAnimClip(L"CrouchWalkR");       break;
        case UnitAnimationType::CROUCH_WALK_L:      animator->SetCurrentAnimClip(L"CrouchWalkL");       break;
        default:
            break;
        }
    }

    animator->Update();
}

void AnimationMove::Render()
{

}
