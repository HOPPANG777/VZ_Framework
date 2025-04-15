#include "stdafx.h"
#include "FloorCollision.h"

void FloorCollision::Update()
{
    if (!player) return;

    std::shared_ptr<BoxCollider> playerCollider = player->GetComponent<BoxCollider>("BoxCollider");
    if (!playerCollider) return;

    std::shared_ptr<BoxCollider> floorCollider = owner->GetComponent<BoxCollider>("BoxCollider");
    if (!floorCollider) return;

    // 플레이어 상태 관리용 PlayerController
    std::shared_ptr<PlayerController> playerController = player->GetComponent<PlayerController>("PlayerController");
    if (!playerController) return;

    // 현재 프레임의 충돌 여부 계산
    bool isCurrentlyColliding = floorCollider->IsColliding(playerCollider.get());

    // static 변수로 이전 프레임의 충돌 여부 저장
    static bool wasColliding = false;

    if (isCurrentlyColliding)
    {
        // 플랫폼의 상단 위치 계산 (플랫폼 중심 + 절반 높이)
        float floorTop = floorCollider->GetCenter().y + floorCollider->GetSize().y * 0.5f;
        // 플레이어의 바닥 위치 계산 (플레이어 중심 - 절반 높이)
        float playerBottom = playerCollider->GetCenter().y - playerCollider->GetSize().y * 0.5f;

        // 만약 플레이어의 바닥이 플랫폼 위로 침투되어 있다면
        if (playerBottom < floorTop)
        {
            // 충돌 상태 유지
            playerController->SetFall(false);
            playerController->SetCollision(true);
            
            // 플레이어의 y 위치 보정: 바닥에 딱 맞도록 (플랫폼의 상단 + 플레이어의 절반 높이)
            Vector3 curPos = player->GetPosition();
            float desiredY = floorTop + playerCollider->GetSize().y * 0.5f;
            player->SetPosition({ curPos.x, desiredY, 0 });
        }
    }
    else // 현재 충돌이 false인 경우
    {
        // 이전 프레임에는 충돌 상태였다면, 이제 충돌이 종료된 것으로 판단하고 낙하 시작
        if (wasColliding)
        {
            // 단, 점프, 중간, 낙하 중이 아니라면 처리 (즉, 플레이어가 지면과의 접촉 상태에서 벗어나면)
            if (!playerController->GetJumping() && !playerController->GetBetween() && !playerController->GetFalling())
            {
                // 플랫폼의 수평 경계를 이용한 판단: 플랫폼의 왼쪽과 오른쪽 x 위치 계산
                float floorLeft = floorCollider->GetCenter().x - floorCollider->GetSize().x * 0.5f;
                float floorRight = floorCollider->GetCenter().x + floorCollider->GetSize().x * 0.5f;
                float playerX = player->GetPosition().x;
                
                cout << floorLeft << endl;
                cout << floorRight << endl;

                // 플레이어의 x좌표가 플랫폼의 좌우 범위를 벗어나면 충돌 종료
                if (playerX < floorLeft || playerX > floorRight)
                {
                    playerController->SetCollision(false);
                    playerController->SetFall(true);
                    playerController->SetGravity(playerController->GetSpeed() * 2.0f);
                    //playerController->SetGravity(200);
                    printf("충돌 종료: 플레이어 낙하 시작\n");
                }
            }
        }
    }

    // 현재 충돌 여부를 이전 충돌 변수에 저장하여 다음 프레임에 사용
    wasColliding = isCurrentlyColliding;

    // -- 도끼(lumberAxe) 충돌 처리는 기존 코드와 비슷하게 처리 --
    if (!lumberAxe) return;

    std::shared_ptr<LumberAxeComponent> axeComponent = lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent");
    if (!axeComponent) return;

    AxeState state = axeComponent->GetState();
    bool isReturningOrIdle =
        (state == AxeState::AXE_RETURNING_R || state == AxeState::AXE_RETURNING_L ||
            state == AxeState::AXE_IDLE_R || state == AxeState::AXE_IDLE_L);
    if (isReturningOrIdle) return;

    std::shared_ptr<BoxCollider> axeCollider = lumberAxe->GetComponent<BoxCollider>("BoxCollider");
    if (!axeCollider) return;

    if (floorCollider->IsColliding(axeCollider.get()))
    {
        float floorTop = floorCollider->GetCenter().y + floorCollider->GetSize().y * 0.5f;
        float axeBottom = axeCollider->GetCenter().y - axeCollider->GetSize().y * 0.5f;
        if (axeBottom < floorTop)
        {
            axeComponent->Land();  // 도끼 착지 처리

            // 위치 보정: 도끼를 플랫폼 위에 맞춤
            float correction = floorTop - axeBottom;
            Vector3 axePos = lumberAxe->GetPosition();
            lumberAxe->SetPosition({ axePos.x, axePos.y + correction, axePos.z });
        }
    }
}
