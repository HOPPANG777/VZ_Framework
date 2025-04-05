#pragma once

class PlayerMove : public Component
{
public:
    PlayerMove(const string& name = "PlayerMove", const float speed = 250.0f) : Component(name), speed(speed) {}
    ~PlayerMove() override = default;

public:
    void Update() override;
    void Render() override;

public:
    // 충돌 감지를 위한 오브젝트 설정
    void SetOverObject(const shared_ptr<Object> object) { overObject = object; }

    // 좌우 이동, 점프, 중력 등 상태를 외부에서 리셋하고 싶으면 Set 메서드도 추가할 수 있습니다.
    void ResetJump() { isJump = isBetween = isFall = false; gravity = originGravity; }

public:
	bool GetRight() const { return isRight; } // 바라보는 방향 반환
	float GetSpeed() const { return currentSpeed; } // 현재 속도 반환

public:
    // 외부에서 충돌 여부를 설정할 수 있도록 하는 함수
    void SetCollision(bool col) { isCollision = col; }
    bool IsCollision() const { return isCollision; }

    // 외부 충돌 검사에서 낙하 상태를 종료할 때 호출 (예: 바닥에 닿았을 때)
    void Land();

private:
    float speed;                     // 이동 속도
	float currentSpeed = 0.0f;       // 현재 속도
    float gravity = 150.0f;          // 중력 가속도
    float originGravity = 150.0f;    // 초기 중력 값
    float jumpVelocity = 0.0f;       // 점프 속도

    bool isJump = false;          // 점프 여부
    bool isBetween = false;       // 점프 최고점 여부
    bool isFall = false;          // 낙하 여부
    bool isCrouch = false;        // 숙이기 여부
    bool rightMove = false;       // 이동 방향
    bool isRight = true;          // 바라보는 방향

    // isCollision: 현재 플레이어가 땅(또는 충돌 오브젝트)와 접촉했는지 여부.
    bool isCollision = false;

    shared_ptr<Object> overObject; // 충돌 감지용 오브젝트
};

