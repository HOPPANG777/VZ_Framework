#include "stdafx.h"
#include "FloorCollision.h"

void FloorCollision::Update()
{
    if (!player) return;

    std::shared_ptr<BoxCollider> playerCollider = player->GetComponent<BoxCollider>("BoxCollider");
    if (!playerCollider) return;

    std::shared_ptr<BoxCollider> floorCollider = owner->GetComponent<BoxCollider>("BoxCollider");
    if (!floorCollider) return;

    std::shared_ptr<PlayerController> playerController = player->GetComponent<PlayerController>("PlayerController");
    if (!playerController) return;

    if (floorCollider->IsColliding(playerCollider.get()))
    {
        // 바닥의 상단 위치 계산 (플랫폼 위쪽)
        float floorTop = floorCollider->GetCenter().y + floorCollider->GetSize().y * 0.5f;
        // 플레이어의 바닥 위치 계산
        float playerBottom = playerCollider->GetCenter().y - playerCollider->GetSize().y * 0.5f;

        // 단순 비교: 플레이어의 하단이 바닥 위로 침투했으면 보정
        if (playerBottom < floorTop)
        {
            // 플레이어 상태 업데이트 (낙하 중 해제 등)
            playerController->SetFall(false);
            playerController->SetCollision(true);

            // 위치 보정: 플레이어의 바닥이 딱 플랫폼 위에 위치하도록
            Vector3 curPos = player->GetPosition();
            float desiredY = floorTop + playerCollider->GetSize().y * 0.5f;
            player->SetPosition({ curPos.x, desiredY, 0 });
        }
    }


    if (!lumberAxe) return;

    std::shared_ptr<LumberAxeComponent> axeComponent = lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent");
    if (!axeComponent) return;

    AxeState state = axeComponent->GetState();
    bool isReturning = (state == AxeState::AXE_RETURNING_R || state == AxeState::AXE_RETURNING_L);
    bool isIdle = (state == AxeState::AXE_IDLE_R || state == AxeState::AXE_IDLE_L);
    if (isReturning || isIdle) return;

    std::shared_ptr<BoxCollider> axeCollider = lumberAxe->GetComponent<BoxCollider>("BoxCollider");
    if (!axeCollider) return;

    if (floorCollider->IsColliding(axeCollider.get()))
    {
        float floorTop = floorCollider->GetCenter().y + floorCollider->GetSize().y * 0.5f;
        float axeBottom = axeCollider->GetCenter().y - axeCollider->GetSize().y * 0.5f;

        if (axeBottom < floorTop)
        {
            axeComponent->Land();  // 착지 처리

            // 위치 보정
            float correction = floorTop - axeBottom;
            Vector3 axePos = lumberAxe->GetPosition();
            lumberAxe->SetPosition({ axePos.x, axePos.y + correction, axePos.z });
        }
    }
}
