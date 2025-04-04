#pragma once

class PlayerMove : public Component
{
public:
    PlayerMove(const string& name = "PlayerMove", const float mSpd = 250.0f) : Component(name), mSpd(mSpd) {}
    ~PlayerMove() override = default;

public:
    void Update() override;
    void Render() override;

public:
    // 충돌 감지를 위한 오브젝트 설정
    void SetOverObject(const shared_ptr<Object> object) { overObject = object; }

    // 좌우 이동, 점프, 중력 등 상태를 외부에서 리셋하고 싶으면 Set 메서드도 추가할 수 있습니다.
    void ResetJump() { mJump = mBetween = mFall = false; mGravity = mGInit; }

public:
    // 외부에서 충돌 여부를 설정할 수 있도록 하는 함수
    void SetCollision(bool col) { isCollision = col; }
    bool IsCollision() const { return isCollision; }

    // 외부 충돌 검사에서 낙하 상태를 종료할 때 호출 (예: 바닥에 닿았을 때)
    void Land();

private:
    float mSpd;                     // 이동 속도
    float mGravity = 150.0f;      // 중력 가속도
    float mGInit = 150.0f;        // 초기 중력 값
    float jumpVelocity = 0.0f;  // 점프 속도

    bool mJump = false;          // 점프 여부
    bool mBetween = false;       // 점프 최고점 여부
    bool mFall = false;          // 낙하 여부
    bool mCrouch = false;        // 숙이기 여부
    bool moveR = false;          // 이동 방향
    bool mRight = true;         // 바라보는 방향

    // isCollision: 현재 플레이어가 땅(또는 충돌 오브젝트)와 접촉했는지 여부.
    bool isCollision = false;

    shared_ptr<Object> overObject; // 충돌 감지용 오브젝트
};

