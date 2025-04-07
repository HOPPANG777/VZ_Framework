#include "stdafx.h"
#include "PushableComponent.h"

void PushableComponent::Update()
{
    // 오브젝트에 Rigidbody나 Transform 컴포넌트가 있다면,
    // 플레이어의 입력(예: 좌우 방향키)를 받아서 오브젝트의 위치를 이동시킵니다.
    if (isBeingPushed)
    {
        // 예시: 단순하게 좌우 입력으로만 이동
        Vector3 pushDir = Vector3::Zero;
        if (Keyboard::Get()->Press('A')) pushDir.x = -1;
        if (Keyboard::Get()->Press('D')) pushDir.x = 1;

        if (pushDir.x != 0)
        {
            pushDir.Normalize();
            // 오브젝트의 Transform 컴포넌트를 업데이트합니다.
            auto transform = owner->GetComponent<Transform>("Transform");
            if (transform)
            {
                Vector3 pos = transform->GetPosition();
                pos += pushDir * pushSpeed * TIME->GetDeltaTime();
                transform->SetPosition(pos);
            }
        }
    }
}
