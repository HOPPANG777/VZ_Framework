#include "stdafx.h"
#include "InteractionComponent.h"

void InteractionComponent::Update()
{
    // E 키가 눌린 순간(KeyDown) – 상호작용 시작 처리
    if (Input::Get()->KeyDown('E'))
    {
        // 만약 현재 상호작용이 진행 중이 아니라면 시작
        if (!isInteracting)
        {
            try
            {
                // 예시로, CollisionManager에서 현재 오너(플레이어)와 충돌 중인 오브젝트 목록을 가져옴
                std::vector<GameObject*> collidingObjects = CollisionManager::Get()->GetCollidingObjects(owner);
                for (GameObject* obj : collidingObjects)
                {
                    // 캡슐화를 위해 GameObject 내부의 컴포넌트 접근은 템플릿 함수를 사용
                    IInteractable* interactable = obj->FindComponentByInterface<IInteractable>();
                    if (interactable)
                    {
                        activeInteractable = interactable; // 현재 상호작용 대상 저장
                        activeInteractable->Interact();      // 상호작용 시작
                        isInteracting = true;  // 상호작용 상태 활성화
                        break;  // 하나의 오브젝트와 상호작용 후 종료 (여러 오브젝트와 상호작용하고 싶다면 break를 제거)
                    }
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "[PlayerInteraction] Interact() 호출 중 예외 발생: " << e.what() << std::endl;
            }
        }
    }

    // E 키가 떼진 순간(KeyUp) – 상호작용 취소 처리, 상호작용이 실제 진행 중일 때만 처리
    if (Input::Get()->KeyUp('E'))
    {
        if (isInteracting)  // 상호작용 중일 때만 처리
        {
            try
            {
                if (activeInteractable)
                {
                    activeInteractable->StopInteract();  // 상호작용 중지 호출
                    activeInteractable = nullptr;          // 대상 초기화
                }
                isInteracting = false;  // 상호작용 상태 해제
            }
            catch (const std::exception& e)
            {
                std::cerr << "[PlayerInteraction] StopInteract() 호출 중 예외 발생: " << e.what() << std::endl;
            }
        }
    }
}