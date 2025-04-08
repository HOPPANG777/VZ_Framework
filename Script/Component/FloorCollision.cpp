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
        float floorTop = floorCollider->GetCenter().y + floorCollider->GetSize().y * 0.5f;
        float playerBottom = playerCollider->GetCenter().y - playerCollider->GetSize().y * 0.5f;

        // 침투 여부 확인
        if (playerBottom < floorTop)
        {
            // 위치 보정
            Vector3 curPos = player->GetPosition();
            float desiredY = floorTop + playerCollider->GetSize().y * 0.5f;
            player->SetPosition({ curPos.x, desiredY, curPos.z });

            // 충돌 상태 설정
            playerController->SetCollision(true);
            playerController->SetFall(false); 
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
