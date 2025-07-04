#pragma once


class IInteractable
{
public:
    virtual ~IInteractable() = default;
    virtual void Interact() = 0; // 상호작용 요청 시 실행할 함수
	virtual void StopInteract() = 0; // 상호작용 중지 요청 시 실행할 함수
};
