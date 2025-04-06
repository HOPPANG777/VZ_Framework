#include "stdafx.h"
#include "WallCollision.h"


// 플레이어와 wall(소유자)의 충돌 판정은 BoxCollider::IsColliding()를 사용한다.
void WallCollision::Update()
{
    if (!player) return;

    // 플레이어의 BoxCollider ("BoxCollider"라는 이름으로 등록)
    auto playerCollider = player->GetComponent<BoxCollider>("BoxCollider");
    if (!playerCollider) return;

    // wall(소유자)의 BoxCollider
    auto wallCollider = owner->GetComponent<BoxCollider>("BoxCollider");
    if (!wallCollider) return;

    // 플레이어와 wall 간의 충돌 검사
    if (wallCollider->IsColliding(playerCollider.get()))
    {
        // wall의 기준 y: owner->GetPosition().y + (벽 높이의 0.6배)
        float wallTop = owner->GetPosition().y + (wallCollider->GetSize().y * 0.6f);
        if (player->GetPosition().y < wallTop)
        {
            // 플레이어의 상태는 PlayerMove 컴포넌트를 통해 조회
            auto pm = player->GetComponent<PlayerController>("PlayerController");
            if (!pm) return;
            // GetRight()로 플레이어가 오른쪽을 향하는지 확인 (true면 오른쪽)
            bool playerFacingRight = pm->GetRight();
            float speed = pm->GetSpeed();
            // 고정 조정 속도 대신 속도 값을 사용

            // 만약 wall이 오른쪽에 있고 플레이어가 왼쪽을 향하고 있다면,
            // 즉, rightWall == true 이고, playerFacingRight == false
            if (rightWall && !playerFacingRight)
            {
                Vector3 newPos = player->GetPosition();
                newPos.x += speed * TIME->GetDeltaTime();
                player->SetPosition(newPos);
            }
            // 반대로, wall이 왼쪽에 있고 플레이어가 오른쪽을 향하고 있다면
            else if (!rightWall && playerFacingRight)
            {
                Vector3 newPos = player->GetPosition();
                newPos.x -= speed * TIME->GetDeltaTime();
                player->SetPosition(newPos);
            }
        }
    }

    // 도끼(LumberAxe) 충돌 검사 (플레이어와의 충돌은 무시)
    if (lumberAxe)
    {
        auto la = lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent");
        if (la)
        {
            AxeState axeState = la->GetState();
            // 도끼의 상태가 반환중 또는 Idle 상태이면 wall 충돌 무시
            if (axeState == AxeState::AXE_RETURNING_R || axeState == AxeState::AXE_RETURNING_L ||
                axeState == AxeState::AXE_IDLE_R || axeState == AxeState::AXE_IDLE_L)
            {
                // 아무 처리도 하지 않음
            }
            else
            {
                auto axeCollider = lumberAxe->GetComponent<BoxCollider>("BoxCollider");
                if (axeCollider && wallCollider->IsColliding(axeCollider.get()))
                {
                    // 도끼의 방향은 GetState()를 통해 판단
                    bool axeFacingRight = (axeState == AxeState::AXE_THROWN_R || axeState == AxeState::AXE_RETURNING_R);
                    float axeSpeed = la->GetSpeed();
                    if (rightWall && !axeFacingRight)
                    {
                        Vector3 newPos = lumberAxe->GetPosition();
                        newPos.x += axeSpeed * TIME->GetDeltaTime();
                        lumberAxe->SetPosition(newPos);
                    }
                    else if (!rightWall && axeFacingRight)
                    {
                        Vector3 newPos = lumberAxe->GetPosition();
                        newPos.x -= axeSpeed * TIME->GetDeltaTime();
                        lumberAxe->SetPosition(newPos);
                    }
                }
            }
        }
    }
}


