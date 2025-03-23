#include "stdafx.h"
#include "FloorCollision.h"

void FloorCollision::Update()
{
    if (!player) return;

    // 플레이어의 BoxCollider 가져오기 ("BoxCollider"라는 이름으로 등록되어 있다고 가정)
    auto playerCollider = player->GetComponent<BoxCollider>("BoxCollider");
    if (!playerCollider) return;

    // owner(해당 floor 오브젝트)의 BoxCollider 가져오기
    auto floorCollider = owner->GetComponent<BoxCollider>("BoxCollider");
    if (!floorCollider) return;

    // 두 Collider 간의 충돌 검사
    if (floorCollider->IsColliding(playerCollider.get()))
    {
        // floor의 상단 위치 계산
        float floorTop = floorCollider->GetCenter().y + (floorCollider->GetSize().y * 0.5f);
        // 플레이어의 하단 위치 계산
        float playerBottom = playerCollider->GetCenter().y - (playerCollider->GetSize().y * 0.5f);

        // 플레이어가 floor에 침투했으면
        if (playerBottom < floorTop)
        {
            // 플레이어의 PlayerMove 컴포넌트의 Land() 호출 (플레이어가 floor에 닿았음을 알림)
            auto pm = player->GetComponent<PlayerMove>("PlayerMove");
            if (pm) pm->Land();

            // 침투 정도만큼 y 보정
            float correction = floorTop - playerBottom;
            Vector3 curPos = player->GetPosition();
            player->SetPosition({ curPos.x, curPos.y + correction, curPos.z });
        }
    }
}
