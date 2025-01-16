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
    if (COLLISION->GetColliderDebug() == true)
    {
        GIZMO->DrawBox(GetCenter(), GetSize(), color, 1.0f);
    }
    else
    {

    }
}
