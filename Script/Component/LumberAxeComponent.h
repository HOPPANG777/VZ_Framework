#pragma once

#include "Enums/AxeState.h"

// 도끼(axe) 컴포넌트 클래스
// - 플레이어(또는 타겟)와 상호작용하며 도끼를 던지고 회수하는 기능 제공
class LumberAxeComponent : public Component
{
public:
    // 생성자: 기본 이름 "LumberAxeComponent"를 사용하며, 초기 상태는 Idle
    LumberAxeComponent(const string& name = "LumberAxeComponent");
    ~LumberAxeComponent() override = default;

public:
    // Component 인터페이스 구현 (업데이트 및 렌더링)
    void Update() override;
    void Render() override;

public:
    // 타겟(플레이어) 설정 함수
    void SetTarget(Object* target) { this->target = target; }

public:
    // 도끼를 던지거나 회수하는 외부 인터페이스
    void Throw(const Vector3& initialVelocity);
    void Recall();

public:
    void Land();

public:
    // 내부 상태 반환
    AxeState GetState() const { return state; }

private:
    // 상태별 업데이트 함수
    void UpdateIdleState(float dt);        // Idle 상태: 타겟 따라가기 및 입력 대기
    void UpdatePrepareState(float dt);     // 던지기 준비 상태: 입력에 따라 던지기 실행 또는 취소
    void UpdateThrownState(float dt);      // 던진 상태: 직선 비행 후 중력 적용하여 이동
    void UpdateReturningState(float dt);   // 회수 상태: 베지어 곡선을 따라 타겟으로 회수

private:
    // 도끼 던짐 관련 함수
    Vector3 ComputeThrowDirection();                                                                                    // 마우스 위치 또는 애니메이션 방향에 따른 던질 방향 계산
    vector<Vector3> SimulateTrajectory(const Vector3& startPos, const Vector3& initialVel, float dt, int numSteps);     // 주어진 초기 조건과 시간 간격에 따라 궤적 시뮬레이션
    void DrawTrajectory(const vector<Vector3>& points, float thickness, const Color& color);                            // 계산된 궤적 점들을 화면에 점선 형태로 그리기
    

private:
    // 유틸리티 함수
    Vector3 QuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t);  // 2차 베지어 곡선 계산 함수
    bool IsFacingLeft(UnitAnimationType currentAnim) const;                                     // 현재 애니메이션 상태를 통해 플레이어가 왼쪽을 바라보는지 판단
    

private:
    AxeState state = AxeState::AXE_IDLE;                            // 현재 도끼 상태 (초기값: Idle)

    // 던진 상태 관련 물리 변수
    Vector3 velocity;                                               // 도끼의 현재 속도
    float gravity = 980.0f;                                         // 중력 가속도
    float straightFlightTime = 0.3f;                                // 던진 후 일정 시간 동안 직선 이동 시간 (중력 무시)

    // 회수 상태 관련 베지어 곡선 변수
    float returnTimer = 0.0f;                                       // 회수 진행 시간 누적
    float returnDuration = 1.0f;                                    // 회수 전체 지속 시간 (초)
    Vector3 returnStart;                                            // 회수 시작 시 도끼 위치
    Vector3 returnControl;                                          // 베지어 곡선 제어점

    Object* target = nullptr;                                       // 도끼를 회수할 타겟(플레이어)
    UnitAnimationType currentAnimation = UnitAnimationType::IDLE_R; // 플레이어 애니메이션 상태 (초기값: 오른쪽 Idle)

    bool isDead = false;                                            // 플레이어가 사망한 상태인지 여부
    bool isLand = false;

};
