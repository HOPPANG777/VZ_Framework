#pragma once

#include "Enum/AxeState.h"

class LumberAxeComponent : public Component
{
public:
    LumberAxeComponent(const string& name = "LumberAxeComponent");
	~LumberAxeComponent() override = default;

public:
	// Component을(를) 통해 상속됨
	void Update() override;
	void Render() override;

public:
	void SetTarget(Object* target) { this->target = target; }

public:
	void Throw(const Vector3& initialVelocity);
	void Recall();

private:
    void UpdateIdleState(float dt);
    void UpdatePrepareState(float dt);
    void UpdateThrownState(float dt);
    void UpdateReturningState(float dt);

private:
    Vector3 ComputeThrowDirection();
    vector<Vector3> SimulateTrajectory(const Vector3& startPos, const Vector3& initialVel, float dt, int numSteps);
    void DrawTrajectory(const vector<Vector3>& points, float thickness, const Color& color);

private:
    // 유틸리티 함수: 2차 베지어 곡선 계산
    Vector3 QuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);

    // 현재 바라보는 방향을 판단 (왼쪽이면 true, 오른쪽이면 false)
    bool IsFacingLeft(UnitAnimationType currentAnim) const;

    // 현재 상태 반환 (예: 디버그용)
    AxeState GetState() const { return state; }

private:
    AxeState state = AxeState::AXE_IDLE; // 초기 상태는 Idle

    // Thrown 상태에서 사용하는 물리 변수
    Vector3 velocity;
    float gravity = 980.0f; // 중력 (필요에 따라 조정)

    // Returning 상태를 위한 베지어 곡선 변수
    float returnTimer = 0.0f;
    float returnDuration = 1.0f; // 귀환 시간 (초)
    Vector3 returnStart;
    Vector3 returnControl; // 제어점

    // 타겟(플레이어) 포인터. 플레이어는 도끼를 잡는 대상.
    Object* target = nullptr;

    // 현재 애니메이션 상태 (플레이어의 애니메이션 시스템에서 전달받을 수 있음)
    UnitAnimationType currentAnimation = UnitAnimationType::IDLE_R;

    // isDead 플래그 (플레이어에서 설정할 수 있음)
    bool isDead = false;

    // **던져진 후 일정 시간 동안 직선 이동할 시간을 저장**
    float straightFlightTime = 0.3f;  // (예시: 0.3초 동안 직선 이동)
};

