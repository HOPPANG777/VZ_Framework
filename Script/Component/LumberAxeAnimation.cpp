#include "stdafx.h"
#include "LumberAxeAnimation.h"

void LumberAxeAnimation::Update()
{
    // owner에서 LumberAxeComponent를 가져옵니다.
    shared_ptr<LumberAxeComponent> axeComp = owner->GetComponent<LumberAxeComponent>("LumberAxeComponent");
    if (!axeComp) return;

    // 현재 도끼 상태 (예: AXE_IDLE_R, AXE_THROWN_L 등)
    AxeState state = axeComp->GetState();

    // owner에서 Animator 컴포넌트를 가져옵니다.
    shared_ptr<Animator> animator = owner->GetComponent<Animator>("Animator");
    if (!animator) return;

    // 상태에 따라 애니메이션 클립 이름을 설정합니다.
    // (아래 clip 이름은 예시이므로, 실제 애니메이션 클립 이름에 맞게 수정하세요.)
    switch (state)
    {
    case AxeState::AXE_IDLE_R:
        animator->SetCurrentAnimClip(L"AxeIdle_R");
        break;
    case AxeState::AXE_IDLE_L:
        animator->SetCurrentAnimClip(L"AxeIdle_L");
        break;
    case AxeState::AXE_PREPARE_R:
        animator->SetCurrentAnimClip(L"AxePrepare_R");
        break;
    case AxeState::AXE_PREPARE_L:
        animator->SetCurrentAnimClip(L"AxePrepare_L");
        break;
    case AxeState::AXE_THROWN_R:
        animator->SetCurrentAnimClip(L"AxeThrown_R");
        break;
    case AxeState::AXE_THROWN_L:
        animator->SetCurrentAnimClip(L"AxeThrown_L");
        break;
    case AxeState::AXE_RETURNING_R:
        animator->SetCurrentAnimClip(L"AxeReturning_R");
        break;
    case AxeState::AXE_RETURNING_L:
        animator->SetCurrentAnimClip(L"AxeReturning_L");
        break;
    default:
        // 다른 상태에 대해서는 기본 Idle 애니메이션을 사용
        animator->SetCurrentAnimClip(L"AxeIdle_R");
        break;
    }
}

void LumberAxeAnimation::Render()
{

}
