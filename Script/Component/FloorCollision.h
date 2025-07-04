#pragma once

#include "LumberAxeComponent.h"
#include "PlayerController.h"
#include "Components/BoxCollider.h"

class FloorCollision : public Component
{
public:
    // player: 충돌 검사 대상인 플레이어 오브젝트 (std::shared_ptr 또는 raw pointer)
    FloorCollision(const std::string& name) : Component(name) {}
    virtual ~FloorCollision() override = default;

public:
    virtual void Update() override;
    virtual void Render() override {} // 렌더링은 필요없으므로 비워두자.

public:
    void SetPlayer(GameObject* player) { this->player = player; }
    GameObject* GetPlayer() const { return this->player; }

    void SetLumberAxe(GameObject* lumberAxe) { this->lumberAxe = lumberAxe; }
    GameObject* GetLumberAxe() const { return this->lumberAxe; }

private:
    // 이전 프레임 충돌 여부 저장
    bool wasColliding = false;
    
    const float EPSILON = 0.01f;
    
    GameObject* player = nullptr;
    GameObject* lumberAxe = nullptr;
};

