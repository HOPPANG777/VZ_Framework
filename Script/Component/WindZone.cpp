#include "stdafx.h"
#include "WindZone.h"

// 헬퍼 함수: 플레이어 위치가 영역 내에 있는지 확인
bool IsInsideRegion(const Vector3& pos, const Vector3& min, const Vector3& max)
{
    return pos.x >= min.x && pos.x <= max.x && pos.y >= min.y && pos.y <= max.y && pos.z >= min.z && pos.z <= max.z;
}

void WindZone::Update()
{
    auto player = SceneManager::Get()->GetPlayer();
    if (!player) return;

    // InteractionComponent를 통해 플레이어가 상호작용 중인지 확인
    auto interactionComp = player->GetComponent<InteractionComponent>("InteractionComponent");
    if (interactionComp && interactionComp->IsInteracting())
    {
        IInteractable* activeInteractable = interactionComp->GetActiveInteractable();
        if (activeInteractable) return;

        // IInteractable 인터페이스를 구현한 객체가 Component임을 가정합니다.
        if (auto interactComp = dynamic_cast<Component*>(activeInteractable))
        {
            // Component가 가지고 있는 GetOwner()를 이용해 상호작용 대상의 소유자를 확인합니다.
            // 플레이어가 PuzzleRock과 상호작용 중이면 바람 효과를 적용하지 않습니다.
            if (interactComp->GetOwner() && interactComp->GetOwner()->GetName() == "PuzzleRock") return;
        }
    }

    // 플레이어가 WindZone 영역 내에 있으면 바람 이동 적용
    Vector3 playerPos = player->GetPosition();
    if (IsInsideRegion(playerPos, mRegionMin, mRegionMax))
    {
        Vector3 displacement = mWindDirection * mIntensity * Time::Get()->GetDeltaTime();
        player->SetPosition(playerPos + displacement);
    }
}

