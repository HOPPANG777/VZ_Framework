#pragma once

// PlayerController는 플레이어의 이동, 점프, 중력, 그리고 애니메이션 상태 전환을 통합 관리합니다.
class PlayerController : public Component
{
public:
    // 생성자: 기본 이동 속도(speed)를 전달 (기본값 250)
    PlayerController(const string& name = "PlayerController", float speed = 250.0f);
    virtual ~PlayerController() override = default;

public:
    virtual void Update() override;
    virtual void Render() override {} // 렌더링은 물리/애니메이션 업데이트에 집중

public:
    // 외부 충돌(플랫폼과의 접촉) 여부를 설정하는 함수.
    void SetCollision(bool col) { isCollision = col; }
	void SetFall(bool fall) { isFall = fall; } // 낙하 상태 설정
	void SetGravity(float g) { gravity = g; } // 중력 설정

public:
    // 상태 접근자
    bool GetCollision() const { return isCollision; }
    bool GetJumping() const { return isJump; }
    bool GetFalling() const { return isFall; }
    bool GetBetween() const { return isBetween; }
    bool GetRight() const { return isRight; }
    float GetSpeed() const { return currentSpeed; }

public:
    // 추가: 플레이어가 사망했을 때 호출
    void SetDead(bool dead) { isDead = dead; }

    // 내부 업데이트 함수
    void UpdateMovement(float dt);
    void UpdateAnimation();

    // Land()를 호출하면 점프/낙하 상태를 리셋
    void Land();

private:
    // 이동/점프 관련 변수
    float speed;          // 기본 이동 속도
    float currentSpeed;   // 현재 속도(상태에 따라 변동)
    float gravity;        // 현재 중력값
    float originGravity;  // 초기 중력값
    float jumpVelocity;   // 점프 시작 시 초기 속도

    // 상태 플래그
    bool isJump;          // 점프 시작 상태
    bool isBetween;       // 최고점(점프 중 정점 근처) 상태
    bool isFall;          // 낙하 상태
    bool isCrouch;        // 숙이기 상태 (추후 확장)
    bool isAtking;        // 공격 상태 (추후 확장)
    bool isRolling;       // 구르는 상태 (추후 확장)
    bool isCollision;     // 외부 충돌(발판과의 접촉) 여부

    bool isRight;         // 바라보는 방향 (true: 오른쪽)

    // 애니메이션 상태 관리
    UnitAnimationType currentAnimation;
    bool isDead;          // 사망 상태

};

