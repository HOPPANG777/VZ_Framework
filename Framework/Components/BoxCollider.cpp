#include "Framework.h"
#include "BoxCollider.h"

void BoxCollider::Update()
{
    if (GetOwner())
    {
        // 소유 객체의 위치를 기반으로 중심을 업데이트
        auto ownerPosition = GetOwner()->GetPosition();
        center = ownerPosition;
    }
}

void BoxCollider::Render()
{
    if (!COLLISION->GetColliderDebug()) return;

    // collider의 center(월드 좌표)
    Vector3 center = GetCenter(); // 예: Transform 등에서 가져옴
    Vector3 cameraPos = CAMERA->GetPosition();

    // 화면 좌표로 변환 (카메라가 이동한 만큼 빼기)
    Vector3 screenCenter = center - cameraPos;

    // 기즈모 그리기
    // size(박스 크기)는 그대로, center만 screenCenter로
    GIZMO->DrawBox(screenCenter, size, color, 1.0f);
}
