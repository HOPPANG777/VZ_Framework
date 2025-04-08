#pragma once

#include "Interface/IInteractable.h"

class InteractionComponent : public Component
{
public:
    InteractionComponent(const string& name = "InteractionComponent") : Component(name) {}
    virtual ~InteractionComponent() override = default;

public:
    virtual void Update() override;
    virtual void Render() override {} // 렌더링은 별도 없음

private:
    // 현재 상호작용 중인 대상의 인터페이스 포인터
    IInteractable* activeInteractable;

    // 상호작용 상태를 나타내는 플래그
    bool isInteracting = false;

};