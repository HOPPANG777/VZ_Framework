#pragma once

#include "LumberAxeComponent.h"
#include "PlayerController.h"

class FloorCollision : public Component
{
public:
    // player: 충돌 검사 대상인 플레이어 오브젝트 (shared_ptr 또는 raw pointer)
    FloorCollision(const string& name, const shared_ptr<Object> player, const shared_ptr<Object> lumberAxe) : Component(name), player(player), lumberAxe(lumberAxe) {}
    virtual ~FloorCollision() override = default;

    virtual void Update() override;
    virtual void Render() override {} // 렌더링은 필요없으므로 비워두자.

private:
    shared_ptr<Object> player;
    shared_ptr<Object> lumberAxe;
    shared_ptr<Object> floorEnd;

};

