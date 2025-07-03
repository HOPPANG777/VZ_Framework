#pragma once

#include "Component.h"
#include "Objects/BaseObject/Object.h"
#include "Systems/Input.h"
#include "Systems/Time.h"

class PlayerMove : public Component
{
public:
    PlayerMove(const std::string& name = "PlayerMove", float spd = 250.0f);
    virtual ~PlayerMove() override = default;

public:
    virtual void Update() override;
    virtual void Render() override;

public:
    // 충돌(땅 접촉) 시 외부에서 설정할 수 있도록
    void SetCollision(bool col) { isCollision = col; }
    bool IsCollision() const { return isCollision; }

public:
    // 플레이어의 이동 관련 값들을 외부에서 사용할 수 있도록
    bool GetRight() const { return isRight; }
    float GetSpeed() const { return currentSpeed; }
    bool IsJumping() const { return isJump; }

public:
    // Land() 호출 시, 땅에 닿았을 때 상태 리셋
    void Land();

private:
    float speed;          // 기본 이동 속도
    float currentSpeed;   // 현재 속도 (상태에 따라 변동)
    float gravity;        // 현재 중력값
    float originGravity;  // 초기 중력값
    float jumpVelocity;   // 점프 시 초기 속도

    bool isJump;          // 점프 중 여부
    bool isBetween;       // 점프 최고점 사이 상태
    bool isFall;          // 낙하 중 여부
    bool isCrouch;        // 숙이기 상태 여부
    bool rightMove;       // 이동 방향 여부 (true: 오른쪽)
    bool isRight;         // 바라보는 방향 (true: 오른쪽)
    bool isAtking;        // 공격 중 여부 (사용 시)
    bool isRolling;       // 구르는 상태 여부 (사용하지 않는다면 false)
    bool isCollision;     // 땅과 충돌(접촉) 여부

    // 충돌 대상(바닥 등)을 설정할 수 있다면 추가
    std::shared_ptr<Object> overObject;
};
