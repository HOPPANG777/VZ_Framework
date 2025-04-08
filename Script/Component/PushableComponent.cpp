#include "stdafx.h"
#include "PushableComponent.h"

void PushableComponent::Update()
{
    if (isBeingPushed)
    {
        Vector3 pushDir = Vector3::Zero;
        if (Keyboard::Get()->Press('A')) pushDir.x = -1;
        if (Keyboard::Get()->Press('D')) pushDir.x = 1;

        if (pushDir.x != 0)
        {
            pushDir.Normalize();
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

void PushableComponent::Interact()
{
    // 상호작용 요청이 오면 밀기를 시작합니다.
    StartPushing();
}

void PushableComponent::StopInteract()
{
	// 상호작용 중지 요청이 오면 밀기를 멈춥니다.
	StopPushing();
}
