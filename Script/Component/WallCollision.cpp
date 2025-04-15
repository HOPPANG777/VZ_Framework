#include "stdafx.h"
#include "WallCollision.h"

// 플레이어와 wall(소유자)의 충돌 판정은 BoxCollider::IsColliding()를 사용한다.
void WallCollision::Update()
{
    if (!player) return;

    // 플레이어의 BoxCollider ("BoxCollider"라는 이름으로 등록)
    const std::shared_ptr<BoxCollider>& playerCollider = player->GetComponent<BoxCollider>("BoxCollider");
    if (!playerCollider) return;

    // wall(소유자)의 BoxCollider
    const std::shared_ptr<BoxCollider>& wallCollider = owner->GetComponent<BoxCollider>("BoxCollider");
    if (!wallCollider) return;

    float wallEdge = 0.0f;
    Vector3 wallPos = owner->GetPosition();
    float halfWidth = wallCollider->GetSize().x * 0.5f;

    // 플레이어와 wall 간의 충돌 검사 (충돌했을 때만 처리)
    if (wallCollider->IsColliding(playerCollider.get()))
    {
        // 벽의 x 방향 기준값 계산:
        // wallEdge는 wall의 중앙 x 좌표에서 벽의 너비의 0.6배를 더하거나 뺀 값으로 판단
        // rightWall == true이면 wall의 왼쪽 경계, false이면 wall의 오른쪽 경계를 사용
        //float wallEdge = 0.0f;
        //Vector3 wallPos = owner->GetPosition();
        //float halfWidth = wallCollider->GetSize().x * 0.5f;
        if (rightWall)
        {
            // 벽이 오른쪽에 있으면, wall의 왼쪽 경계를 기준으로 함
            wallEdge = wallPos.x - halfWidth + (wallCollider->GetSize().x * 0.6f);
        }
        else
        {
            // 벽이 왼쪽에 있으면, wall의 오른쪽 경계를 기준
            wallEdge = wallPos.x + halfWidth - (wallCollider->GetSize().x * 0.6f);
        }

        // 플레이어의 x 좌표 비교하여 충돌 보정
        float playerX = player->GetPosition().x;

        // 플레이어 컨트롤러에서 방향과 속도 값 얻기
        const std::shared_ptr<PlayerController>& pm = player->GetComponent<PlayerController>("PlayerController");
        if (!pm) return;
        bool playerFacingRight = pm->GetRight();
        float speed = pm->GetSpeed();

        // 플레이어가 벽의 내측으로 침투했으면 x 보정 실행
        if (rightWall && playerX < wallEdge && !playerFacingRight)
        {
            Vector3 newPos = player->GetPosition();
            // 롤 중이면 속도 2배 적용
 /*           if (player->GetmRoll())
                newPos.x += (speed * 2) * TIME->GetDeltaTime();
            else*/
                newPos.x += speed * TIME->GetDeltaTime();
            player->SetPosition(newPos);
        }
        else if (!rightWall && playerX > wallEdge && playerFacingRight)
        {
            Vector3 newPos = player->GetPosition();
            //if (player->GetmRoll())
            //    newPos.x -= (speed * 2) * TIME->GetDeltaTime();
            //else
                newPos.x -= speed * TIME->GetDeltaTime();
            player->SetPosition(newPos);
        }
    }

    // 도끼(LumberAxe) 충돌 검사 (플레이어와의 충돌은 무시)
    if (lumberAxe)
    {
        const std::shared_ptr<LumberAxeComponent>& la = lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent");
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
                const std::shared_ptr<BoxCollider>& axeCollider = lumberAxe->GetComponent<BoxCollider>("BoxCollider");
                if (axeCollider && wallCollider->IsColliding(axeCollider.get()))
                {
                    // 도끼의 방향은 GetState()를 통해 판단
                    bool axeFacingRight = (axeState == AxeState::AXE_THROWN_R || axeState == AxeState::AXE_RETURNING_R);
                    float axeSpeed = la->GetSpeed();
                    float axeX = lumberAxe->GetPosition().x;

                    if (rightWall && axeX < wallPos.x - halfWidth + (wallCollider->GetSize().x * 0.6f) && !axeFacingRight)
                    {
                        Vector3 newPos = lumberAxe->GetPosition();
                        newPos.x += axeSpeed * TIME->GetDeltaTime();
                        lumberAxe->SetPosition(newPos);
                    }
                    else if (!rightWall && axeX > wallPos.x + halfWidth - (wallCollider->GetSize().x * 0.6f) && axeFacingRight)
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
