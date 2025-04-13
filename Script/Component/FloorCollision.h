#pragma once

#include "LumberAxeComponent.h"
#include "PlayerController.h"

class FloorCollision : public Component
{
public:
    // player: 충돌 검사 대상인 플레이어 오브젝트 (shared_ptr 또는 raw pointer)
    FloorCollision(const string& name) : Component(name) {}
    virtual ~FloorCollision() override = default;

public:
    virtual void Update() override;
    virtual void Render() override {} // 렌더링은 필요없으므로 비워두자.

public:
    void SetPlayer(Object* player) { this->player = player; }
    Object* GetPlayer() const { return this->player; }

    void SetLumberAxe(Object* lumberAxe) { this->lumberAxe = lumberAxe; }
    Object* GetLumberAxe() const { return this->lumberAxe; }

private:
    Object* player;
    Object* lumberAxe;

};

