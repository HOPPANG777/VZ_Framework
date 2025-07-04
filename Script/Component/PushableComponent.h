#pragma once

#include "Interface/IInteractable.h"
#include "Objects/BaseObject/GameObject.h"
#include "Components/Component.h"
#include "Components/Transform.h"
#include "Systems/Input.h"
//#include "Systems/Time.h"

class PushableComponent : public Component, public IInteractable
{
public:
    PushableComponent(const std::string& name = "PushableComponent", float pushSpeed = 250.0f) : Component(name), pushSpeed(pushSpeed), isBeingPushed(false) {}
    virtual ~PushableComponent() override = default;

public:
    virtual void Update() override;
    virtual void Render() override {} // 렌더링은 필요 없음

public:
    // IInteractable 인터페이스 구현
    virtual void Interact() override;
	virtual void StopInteract() override;

public:
    // 내부 밀기 동작 시작/종료
    void StartPushing() { isBeingPushed = true; }
    void StopPushing() { isBeingPushed = false; }

private:
    float pushSpeed;
    bool isBeingPushed;

};



