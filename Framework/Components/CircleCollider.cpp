#include "Framework.h"
#include "CircleCollider.h"

void CircleCollider::Update()
{
    if (GetOwner())
    {
        // 소유 객체의 위치를 기반으로 중심을 업데이트
        auto ownerPosition = GetOwner()->GetPosition();
        center = ownerPosition;
    }
}

void CircleCollider::Render()
{
    if (COLLISION->GetColliderDebug() == true)
    {
        GIZMO->DrawCircle(GetCenter(), GetRadius(), color, 1.0f);
    }
    else
    {

    }
}
