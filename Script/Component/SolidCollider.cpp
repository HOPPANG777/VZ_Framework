#include "stdafx.h"
#include "SolidCollider.h"


// Update(): Collider의 위치는 소유 객체의 위치 + offset으로 갱신
void SolidCollider::Update()
{
    // 여기서는 별도의 내부 변수 없이 GetCenter()로 계산하도록 함.
    // 필요에 따라 Collider의 AABB 데이터를 갱신할 수 있음.
}

// Render(): 디버그용으로 충돌 영역 사각형을 그립니다.
void SolidCollider::Render()
{
    // Gizmo를 이용하여 사각형 그리기 (디버그 용도)
    Vector3 center = GetCenter();
    Vector3 half = GetHalfSize();
    Vector3 topLeft = center - half;
    Vector3 bottoisRight = center + half;
    // 예: Gizmo에서 DrawBox()를 호출해 사각형 경계선 표시
    GIZMO->DrawBox(center, size, color, 1.0f);
}

// GetCenter(): 소유 객체 위치 + offset 반환
Vector3 SolidCollider::GetCenter() const
{
    return GetOwner()->GetPosition() + offset;
}

// GetHalfSize(): size의 절반 계산
Vector3 SolidCollider::GetHalfSize() const
{
    return size * 0.5f;
}

// ResolveCollision(): 다른 Collider와의 AABB 충돌을 해소
// 여기서는 간단하게 두 AABB의 겹침 정도를 계산하고, 더 작은 축 방향으로 분리하는 예제
void SolidCollider::ResolveCollision(Collider* other)
{
    // 이 예제는 다른 Collider가 SolidCollider라고 가정합니다.
    SolidCollider* otherSolid = dynamic_cast<SolidCollider*>(other);
    if (!otherSolid) return;

    Vector3 myCenter = GetCenter();
    Vector3 otherCenter = otherSolid->GetCenter();
    Vector3 myHalf = GetHalfSize();
    Vector3 otherHalf = otherSolid->GetHalfSize();

    // 두 AABB의 겹침을 축별로 계산
    float deltaX = myCenter.x - otherCenter.x;
    float overlapX = myHalf.x + otherHalf.x - abs(deltaX);

    float deltaY = myCenter.y - otherCenter.y;
    float overlapY = myHalf.y + otherHalf.y - abs(deltaY);

    // 겹침이 없으면 return
    if (overlapX <= 0 || overlapY <= 0) return;

    // 더 작은 겹침 방향을 선택해 분리 벡터 계산
    if (overlapX < overlapY)
    {
        // X축 방향 분리
        float pushX = (deltaX < 0) ? -overlapX : overlapX;
        GetOwner()->SetPosition(GetOwner()->GetPosition() + Vector3(pushX, 0, 0));
    }
    else
    {
        // Y축 방향 분리
        float pushY = (deltaY < 0) ? -overlapY : overlapY;
        GetOwner()->SetPosition(GetOwner()->GetPosition() + Vector3(0, pushY, 0));
    }
}