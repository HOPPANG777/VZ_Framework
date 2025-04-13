#pragma once

#include "Component/LumberAxeComponent.h"
#include "PlayerController.h"

class WallCollision : public Component
{
public:
    // player: 충돌 검사 대상인 플레이어 오브젝트,
    WallCollision(const string& name, bool isRight) : Component(name), rightWall(isRight) {}
    virtual ~WallCollision() override = default;

public:
    virtual void Update() override;
    virtual void Render() override {} // 렌더링은 필요 없으므로 빈 구현

public:
    void SetPlayer(Object* player) { this->player = player; }
    Object* GetPlayer() const { return this->player; }

    void SetLumberAxe(Object* lumberAxe) { this->lumberAxe = lumberAxe; }
    Object* GetLumberAxe() const { return this->lumberAxe; }

private:
    Object* player = nullptr;
    Object* lumberAxe = nullptr;
    bool rightWall; // true: 오른쪽 벽, false: 왼쪽 벽

};


