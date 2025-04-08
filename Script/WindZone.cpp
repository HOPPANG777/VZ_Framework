#include "stdafx.h"
#include "WindZone.h"

void WindZone::Update()
{
    // 예시로, 특정 씬의 플레이어를 전역에서 접근하는 방식 (예: SCENE->GetPlayer())
    // 또는 owner나 게임매니저 등을 통해 현재 플레이어 객체를 얻을 수도 있음.
    const std::shared_ptr<Object>& player = SCENE->GetPlayer();

    if (player)
    {
        Vector3 playerPos = player->GetPosition();
        // 플레이어가 WindZone 내에 있으면
        if (IsInsideRegion(playerPos, mRegionMin, mRegionMax))
        {
            // 바람 방향과 세기에 따라 플레이어의 위치를 업데이트합니다.
            Vector3 displacement = mWindDirection * mIntensity * TIME->GetDeltaTime();
            player->SetPosition(playerPos + displacement);

            // 또는 만약 물리 기반 이동(velocity 적용)을 한다면 
            // player->AddForce(mWindDirection * mIntensity);
        }
    }
}

// 헬퍼 함수: 플레이어 위치가 영역 내에 있는지 확인
bool IsInsideRegion(const Vector3& pos, const Vector3& min, const Vector3& max)
{
    return pos.x >= min.x && pos.x <= max.x &&
        pos.y >= min.y && pos.y <= max.y &&
        pos.z >= min.z && pos.z <= max.z;
}
