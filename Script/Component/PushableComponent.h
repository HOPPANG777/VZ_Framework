#pragma once

class PushableComponent : public Component
{
public:
    PushableComponent(const string& name = "PushableComponent", float pushSpeed = 100.0f) : Component(name), pushSpeed(pushSpeed), isBeingPushed(false) {}
    virtual ~PushableComponent() override = default;

public:
    virtual void Update() override;
    virtual void Render() override {} // 별도 렌더링은 필요 없음

public:
    // 외부에서 밀기 시작/종료를 제어하는 함수
    void StartPushing() { isBeingPushed = true; }
    void StopPushing() { isBeingPushed = false; }

public:
    bool IsPushing() const { return isBeingPushed; }
    // 밀기 속도 반환
    float GetPushSpeed() const { return pushSpeed; }

private:
    float pushSpeed;
    bool isBeingPushed;
    
};


