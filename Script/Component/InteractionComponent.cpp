#include "stdafx.h"
#include "InteractionComponent.h"

void InteractionComponent::Update()
{
    // E키가 눌렸을 때 근처 상호작용 대상의 Interact() 호출
    if (INPUT->KeyPressed('E'))
    {
        // 예시: CollisionManager에서 현재 오너(플레이어)가 충돌 중인 오브젝트들을 반환한다고 가정
        vector<Object*> collidingObjects = COLLISION->GetCollidingObjects(owner);
        for (Object* obj : collidingObjects)
        {
            // 캡슐화를 위해 Object 내부의 components에 직접 접근하지 않고 템플릿 함수를 사용
            IInteractable* interactable = obj->FindComponentByInterface<IInteractable>();
            if (interactable)
            {
                interactable->Interact();
                break; // 한 오브젝트와 상호작용 후 종료 (필요에 따라 여러 오브젝트에 대해 호출 가능)
            }
        }
    }
    else
    {
		// E키가 눌리지 않았을 때 상호작용 중지
		// 예를 들어, 상호작용 UI를 숨기거나 상태를 초기화하는 로직을 추가할 수 있습니다.
        // 예시: CollisionManager에서 현재 오너(플레이어)가 충돌 중인 오브젝트들을 반환한다고 가정
        vector<Object*> collidingObjects = COLLISION->GetCollidingObjects(owner);
        for (Object* obj : collidingObjects)
        {
            // 캡슐화를 위해 Object 내부의 components에 직접 접근하지 않고 템플릿 함수를 사용
            IInteractable* interactable = obj->FindComponentByInterface<IInteractable>();
            if (interactable)
            {
                interactable->StopInteract();
                break; // 한 오브젝트와 상호작용 후 종료 (필요에 따라 여러 오브젝트에 대해 호출 가능)
            }
        }
    }
}