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

};