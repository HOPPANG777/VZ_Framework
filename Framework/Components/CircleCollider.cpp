#include "Framework.h"
#include "CircleCollider.h"

void CircleCollider::Update()
{
    if (GetOwner())
    {
        // 소유 객체의 위치를 기반으로 중심을 업데이트
        Vector3 ownerPosition = GetOwner()->GetPosition();
        center = ownerPosition;
    }
}

void CircleCollider::Render()
{
    if (!CollisionManager::Get()->GetColliderDebug()) return;

    // collider의 center(월드 좌표)
    Vector3 center = GetCenter(); // 예: Transform 등에서 가져옴
    Vector3 cameraPos = Camera::Get()->GetPosition();

    // 화면 좌표로 변환 (카메라가 이동한 만큼 빼기)
    Vector3 screenCenter = center - cameraPos;

    // 기즈모 그리기
    // size(박스 크기)는 그대로, center만 screenCenter로
    Gizmo::Get()->DrawCircle(screenCenter, radius, color, 1.0f);
}
