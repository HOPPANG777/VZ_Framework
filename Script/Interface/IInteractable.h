#pragma once

class IInteractable
{
public:
    virtual ~IInteractable() = default;

    // 플레이어가 상호작용 시 호출되는 함수
    virtual void Interact() = 0;
};
