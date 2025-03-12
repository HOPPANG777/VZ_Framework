#include "Framework.h"
#include "AnimationMove.h"

void AnimationMove::Update()
{
    // Animator 컴포넌트를 가져옵니다.
    auto animator = owner->GetComponent<Animator>("Animator");
    if (!animator) return;

    // 새로운 애니메이션 상태를 결정합니다.
    UnitAnimationType newState = currentAnimation;

    // 숙이기 (Crouch) 관련 입력: S 키 입력을 우선으로 처리
    if (INPUT->KeyPressed('S'))
    {
        // 만약 좌우 이동 입력도 있다면 CrouchWalk 상태, 없다면 단순 Crouch 상태
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
            // 이전 방향에 따라 Crouch 상태 선택
            if (currentAnimation == UnitAnimationType::RUN_L || currentAnimation == UnitAnimationType::IDLE_L)
                newState = UnitAnimationType::CROUCH_L;
            else
                newState = UnitAnimationType::CROUCH_R;
        }
    }
    // 좌우 이동 입력 처리 (숙이는 입력이 없는 경우)
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
        // 아무 입력이 없으면, 이전 방향에 따라 Idle 상태 선택
        if (currentAnimation == UnitAnimationType::RUN_L || currentAnimation == UnitAnimationType::CROUCH_L || currentAnimation == UnitAnimationType::CROUCH_WALK_L)
            newState = UnitAnimationType::IDLE_L;
        else
            newState = UnitAnimationType::IDLE_R;
    }

    // 점프 입력 처리: W 키 (실제 이동은 PlayerMove에서 처리)
    if (INPUT->KeyDown('W'))
    {
        if (currentAnimation == UnitAnimationType::RUN_L || currentAnimation == UnitAnimationType::IDLE_L || currentAnimation == UnitAnimationType::CROUCH_L || currentAnimation == UnitAnimationType::CROUCH_WALK_L)
            newState = UnitAnimationType::JUMP_L;
        else
            newState = UnitAnimationType::JUMP_R;
    }

    // 도끼 관련 입력 처리: 마우스 오른쪽 버튼으로 던지기 준비, E 키로 귀환
    if (INPUT->KeyDown(VK_RBUTTON))
    {
        newState = UnitAnimationType::AXE_THROW_PREPARE;
    }
    if (INPUT->KeyDown('E'))
    {
        newState = UnitAnimationType::AXE_RETURN;
    }

    // 상태가 변경되었다면 Animator에 반영
    if (newState != currentAnimation)
    {
        currentAnimation = newState;
        switch (currentAnimation)
        {
        case UnitAnimationType::IDLE_R:
            animator->SetCurrentAnimClip(L"IdleR");
            break;
        case UnitAnimationType::IDLE_L:
            animator->SetCurrentAnimClip(L"IdleL");
            break;
        case UnitAnimationType::RUN_R:
            animator->SetCurrentAnimClip(L"RunR");
            break;
        case UnitAnimationType::RUN_L:
            animator->SetCurrentAnimClip(L"RunL");
            break;
        case UnitAnimationType::JUMP_R:
            animator->SetCurrentAnimClip(L"JumpR");
            break;
        case UnitAnimationType::JUMP_L:
            animator->SetCurrentAnimClip(L"JumpL");
            break;
        case UnitAnimationType::AXE_THROW_PREPARE:
            animator->SetCurrentAnimClip(L"AxeThrowPrepare");
            break;
        case UnitAnimationType::AXE_RETURN:
            animator->SetCurrentAnimClip(L"AxeReturn");
            break;
            // 숙이는 상태들
        case UnitAnimationType::CROUCH_R:
            animator->SetCurrentAnimClip(L"CrouchR");
            break;
        case UnitAnimationType::CROUCH_L:
            animator->SetCurrentAnimClip(L"CrouchL");
            break;
        case UnitAnimationType::CROUCH_WALK_R:
            animator->SetCurrentAnimClip(L"CrouchWalkR");
            break;
        case UnitAnimationType::CROUCH_WALK_L:
            animator->SetCurrentAnimClip(L"CrouchWalkL");
            break;
        default:
            break;
        }
    }

    // 매 프레임 Animator 업데이트 (델타 타임 전달)
    animator->Update();
}

void AnimationMove::Render()
{

}
