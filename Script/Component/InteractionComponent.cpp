#include "stdafx.h"
#include "InteractionComponent.h"

void InteractionComponent::Update()
{
    // E키가 눌렸을 때 근처 상호작용 대상의 Interact() 호출
    if (INPUT->KeyDown('E'))
    {
        // IInteractable 인터페이스를 구현한 컴포넌트를 찾음
        IInteractable* interactable = owner->FindComponentByInterface<IInteractable>();
        if (interactable)
        {
            interactable->Interact();
        }
    }
}