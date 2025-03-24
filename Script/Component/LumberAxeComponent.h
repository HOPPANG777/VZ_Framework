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
    // 유틸리티 함수: 2차 베지어 곡선 계산
    Vector3 QuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);

    // 현재 바라보는 방향을 판단 (왼쪽이면 true, 오른쪽이면 false)
    bool IsFacingLeft(UnitAnimationType currentAnim) const;

private:
    // 현재 상태 (Idle, Thrown, Returning)
	AxeState state = AxeState::AXE_IDLE;

    // 물리 관련 변수 (Thrown 상태)
    Vector3 velocity;
    float gravity = 980.0f; // 중력 (필요에 따라 조정)

    // Returning 상태를 위한 베지어 곡선 변수
    float returnTimer = 0.0f;
    float returnDuration = 1.0f; // 돌아오는 데 걸리는 시간 (초)
    Vector3 returnStart;
    Vector3 returnControl; // 베지어 곡선의 제어점

    // 타겟 오브젝트 (플레이어)
    Object* target = nullptr;

};

