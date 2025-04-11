#include "stdafx.h"
#include "LadderComponent.h"

void LadderComponent::Update()
{
    // 현재 씬에서 플레이어를 가져옵니다.
    auto player = SCENE->GetPlayer();
    if (!player) return;

    // LadderComponent가 붙은 오브젝트(사다리)에서 BoxCollider를 가져옵니다.
    auto ladderCollider = owner->GetComponent<BoxCollider>("BoxCollider");
    if (!ladderCollider) return;

    // 플레이어 오브젝트에서 BoxCollider를 가져옵니다.
    auto playerCollider = player->GetComponent<BoxCollider>("BoxCollider");
    if (!playerCollider) return;

    // 플레이어와 사다리의 BoxCollider가 충돌하고 있다면
    if (!ladderCollider->IsColliding(playerCollider.get())) return;

    // 만약 플레이어가 D 키를 누른다면
    if (INPUT->KeyPressed('D'))
    {
        // 플레이어의 Transform 컴포넌트를 통해 위치를 수정해 위로 이동시킵니다.
        auto playerTransform = player->GetComponent<Transform>("Transform");
        if (playerTransform)
        {
            const float climbSpeed = 100.0f; // 사다리 오르기 속도 (픽셀/초)
            Vector3 pos = playerTransform->GetPosition();
            pos.y += climbSpeed * TIME->GetDeltaTime();
            playerTransform->SetPosition(pos);
        }
    }
}
