#include "stdafx.h"
#include "LadderComponent.h"

void LadderComponent::Update()
{
    // 현재 씬에서 플레이어를 가져옵니다.
    // LadderComponent가 붙은 오브젝트(사다리)에서 BoxCollider를 가져옵니다.
    // 플레이어 오브젝트에서 BoxCollider를 가져옵니다.
    // 플레이어 컨트롤러를 가져옵니다.
    auto player = SceneManager::Get()->GetPlayer();
    auto ladderCollider = owner->GetComponent<BoxCollider>("BoxCollider");
    auto playerCollider = player->GetComponent<BoxCollider>("BoxCollider");
    auto playerController = player->GetComponent<PlayerController>("PlayerController");

    if (!player || !ladderCollider || !playerCollider || !playerController) return;

    // 플레이어와 사다리의 BoxCollider가 충돌하고 있다면
    //if (!ladderCollider->IsColliding(playerCollider.get())) return;

    bool onLadder = ladderCollider->IsColliding(playerCollider.get());
    if (!onLadder)
    {
        // 더 이상 사다리 위에 있지 않음 → Climbing 비활성화
        playerController->SetClimbing(false);
        return;
    }

    // 만약 플레이어가 W 키를 누른다면
    if (Input::Get()->KeyPressed('W'))
    {
        // 사다리 오르기 시작
        playerController->SetClimbing(true);
        playerController->SetCollision(false);
        playerController->SetFall(false);

        // 플레이어의 Transform 컴포넌트를 통해 위치를 수정해 위로 이동시킵니다.
        auto playerTransform = player->GetComponent<Transform>("Transform");
        if (playerTransform)
        {
            const float climbSpeed = 300.0f;
            Vector3 pos = playerTransform->GetPosition();
            pos.y += climbSpeed * Time::Get()->GetDeltaTime();
            playerTransform->SetPosition(pos);
        }
    }
}
