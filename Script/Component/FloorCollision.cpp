#include "stdafx.h"
#include "FloorCollision.h"

void FloorCollision::Update()
{
    if (!player) return;

    auto playerCollider = player->GetComponent<BoxCollider>("BoxCollider");
    auto floorCollider = owner->GetComponent<BoxCollider>("BoxCollider");
    auto playerController = player->GetComponent<PlayerController>("PlayerController");
    if (!playerCollider || !floorCollider || !playerController) return;

    bool was = wasColliding;
    bool now = floorCollider->IsColliding(playerCollider.get());

    // 오르기 시작
    if (playerController && playerController->GetClimbing()) return;

    // (1) 착지 순간에만 Land()
    if (now && !was)
    {
        playerController->Land();
    }

    if (now)
    {
        // 플랫폼 위에서 collision 유지 및 위치 보정
        playerController->SetCollision(true);
        playerController->SetFall(false);

        float floorTop = floorCollider->GetCenter().y + floorCollider->GetSize().y * 0.5f;
        float playerBottom = playerCollider->GetCenter().y - playerCollider->GetSize().y * 0.5f;
        float pen = floorTop - playerBottom;

        //if (pen > EPSILON)
        if (playerBottom < floorTop)
        {
            Vector3 pos = player->GetPosition();
            pos.y = floorTop + playerCollider->GetSize().y * 0.5f;
            player->SetPosition(pos);
        }
    }
    else if (was)
    {
        // 플랫폼 벗어나는 순간 한 번만 낙하 시작
        if (!playerController->GetJumping() && !playerController->GetBetween() && !playerController->GetFalling())
        {
            playerController->SetCollision(false);
            playerController->SetFall(true);
            playerController->SetGravity(playerController->GetSpeed() * 2.0f);
        }
    }

    // 다음 프레임을 위해 상태 저장
    wasColliding = now;

    // -- 도끼 충돌 처리 (변경 없음) --
    if (!lumberAxe) return;
    auto axeComponent = lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent");
    AxeState state = axeComponent->GetState();
    bool isIdleOrReturn = (state == AxeState::AXE_RETURNING_R || state == AxeState::AXE_RETURNING_L || state == AxeState::AXE_IDLE_R || state == AxeState::AXE_IDLE_L);
    auto axeCollider = lumberAxe->GetComponent<BoxCollider>("BoxCollider");
    if (!axeComponent || isIdleOrReturn || !axeCollider) return;

    if (floorCollider->IsColliding(axeCollider.get()))
    {
        float floorTop = floorCollider->GetCenter().y + floorCollider->GetSize().y * 0.5f;
        float axeBottom = axeCollider->GetCenter().y - axeCollider->GetSize().y * 0.5f;

        if (axeBottom < floorTop)
        {
            axeComponent->Land();
            float correction = floorTop - axeBottom;
            Vector3 axePos = lumberAxe->GetPosition();
            lumberAxe->SetPosition({ axePos.x, axePos.y + correction, axePos.z });
        }
    }
}
