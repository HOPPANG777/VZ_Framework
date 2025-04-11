#pragma once

#include "LumberAxeComponent.h"
#include "PlayerController.h"

class FloorCollision : public Component
{
public:
    // player: 충돌 검사 대상인 플레이어 오브젝트 (shared_ptr 또는 raw pointer)
    FloorCollision(const string& name, Object* player, Object* lumberAxe) : Component(name), player(player), lumberAxe(lumberAxe) {}
    virtual ~FloorCollision() override = default;

public:
    virtual void Update() override;
    virtual void Render() override {} // 렌더링은 필요없으므로 비워두자.

private:
    Object* player;
    Object* lumberAxe;

};

