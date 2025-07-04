#pragma once

#include "HitComponent.h"
#include "Components/BoxCollider.h"

class LogPathCreator : public Component
{
public:
    LogPathCreator(const std::string& name = "LogPathCreator") : Component(name), collisionCount(0), transformed(false), newColliderSize(400, 100, 0) {}
    virtual ~LogPathCreator() override = default;

public:
    virtual void Update() override {}
    virtual void Render() override {}

public:
    // 도끼와 충돌 시 호출될 함수
    void OnAxeCollision();
	void OnLogCollision(Collider* other);

public:
	bool GetTransformed() const { return transformed; }

private:
    int collisionCount;    // 도끼와 충돌한 횟수
    bool transformed;      // 이미 변환이 완료되었는지 여부
    Vector3 newColliderSize; // 새로 적용할 BoxCollider 크기
};

