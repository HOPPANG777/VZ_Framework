#include "stdafx.h"
#include "FloorCollision.h"

void FloorCollision::Update()
{
    if (!player) return;

    // 플레이어의 BoxCollider ("BoxCollider"라는 이름으로 등록되어 있다고 가정)
    auto playerCollider = player->GetComponent<BoxCollider>("BoxCollider");
    if (!playerCollider) return;

    // Floor(해당 floor 오브젝트)의 BoxCollider
    auto floorCollider = owner->GetComponent<BoxCollider>("BoxCollider");
    if (!floorCollider) return;

    // 플레이어와 floor 간의 충돌 검사
    if (floorCollider->IsColliding(playerCollider.get()))
    {
        // floor의 상단 위치 계산
        float floorTop = floorCollider->GetCenter().y + (floorCollider->GetSize().y * 0.5f);
        // 플레이어의 하단 위치 계산
        float playerBottom = playerCollider->GetCenter().y - (playerCollider->GetSize().y * 0.5f);

        // 플레이어가 floor에 침투했으면
        if (playerBottom < floorTop)
        {
            // 플레이어의 PlayerMove 컴포넌트에 충돌 상태를 설정
            auto pm = player->GetComponent<PlayerMove>("PlayerMove");
            if (pm) pm->SetCollision(true);

            // 침투 정도만큼 y 보정 (이 보정은 플레이어가 Update()에서 Land() 호출 후에도 적용될 수 있음)
            float correction = floorTop - playerBottom;
            Vector3 curPos = player->GetPosition();
            player->SetPosition({ curPos.x, curPos.y + correction, curPos.z });
        }
    }

    // 이제 도끼(LumberAxe) 충돌 검사 (플레이어와의 충돌은 무시)
    if (lumberAxe)  // 'lumberAxe'는 미리 설정된 도끼 Object
    {
        auto la = lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent");
        if (la && (la->GetState() == AxeState::AXE_RETURNING_R || la->GetState() == AxeState::AXE_RETURNING_L) || la && (la->GetState() == AxeState::AXE_IDLE_R || la->GetState() == AxeState::AXE_IDLE_L)) return;

        auto axeCollider = lumberAxe->GetComponent<BoxCollider>("BoxCollider");
        if (axeCollider && floorCollider->IsColliding(axeCollider.get()))
        {
            // floor의 상단 계산
            float floorTop = floorCollider->GetCenter().y + (floorCollider->GetSize().y * 0.5f);
            // 도끼의 하단 계산
            float axeBottom = axeCollider->GetCenter().y - (axeCollider->GetSize().y * 0.5f);
            if (axeBottom < floorTop)
            {
                if (la) la->Land();

                // 도끼 위치 보정
                float correction = floorTop - axeBottom;
                Vector3 curPos = lumberAxe->GetPosition();
                lumberAxe->SetPosition({ curPos.x, curPos.y + correction, curPos.z });
            }
        }
    }
}
