#include "stdafx.h"
#include "LumberAxeComponent.h"

// 생성자: 초기 상태, 속도, 중력, 직선 이동 시간 설정
LumberAxeComponent::LumberAxeComponent(const string& name)
    : Component(name), state(AxeState::AXE_IDLE), velocity(Vector3::Zero), gravity(980.0f), returnDuration(1.0f), straightFlightTime(0.3f)
{

}

// 매 프레임 호출되는 업데이트 함수
// - isDead 플래그가 설정되면 업데이트 중단
// - 현재 상태에 따라 각각의 업데이트 함수를 호출함
void LumberAxeComponent::Update()
{
    float dt = DELTA;
    if (isDead) return;

    switch (state)
    {
    case AxeState::AXE_IDLE:
        UpdateIdleState(dt);
        break;
    case AxeState::AXE_PREPARE:
        UpdatePrepareState(dt);
        break;
    case AxeState::AXE_THROWN:
        UpdateThrownState(dt);
        break;
    case AxeState::AXE_RETURNING:
        UpdateReturningState(dt);
        break;
    }
}

// 렌더링 함수: 준비 상태에서 마우스 방향에 따른 도끼 궤적 미리보기 그리기
void LumberAxeComponent::Render()
{
    if (state == AxeState::AXE_PREPARE)
    {
        // 1. 던질 방향 계산 (마우스 위치 기반)
        Vector3 throwDir = ComputeThrowDirection();

        // 2. 초기 속도 계산 (고정 속도 값 사용)
        float throwSpeed = 1000.0f;
        Vector3 initialVel = throwDir * throwSpeed;

        // 3. 궤적 시뮬레이션: 충분한 점을 계산하여 궤적 생성 (dt=0.02, 64단계)
        vector<Vector3> trajPoints = SimulateTrajectory(owner->GetPosition(), initialVel, 0.02f, 64);

        // 4. 궤적 점들을 점선으로 그리기
        DrawTrajectory(trajPoints, 3.0f, WHITE);
    }
}

// 2차 베지어 곡선 계산 함수
// - p0, p1, p2: 곡선의 시작, 제어, 종료 점
// - t: [0,1] 구간의 파라미터
Vector3 LumberAxeComponent::QuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t)
{
    float oneMinusT = 1.0f - t;
    return p0 * (oneMinusT * oneMinusT) + p1 * (2 * oneMinusT * t) + p2 * (t * t);
}

// 현재 애니메이션 상태를 통해 플레이어가 왼쪽을 바라보는지 판단
bool LumberAxeComponent::IsFacingLeft(UnitAnimationType currentAnim) const
{
    switch (currentAnim)
    {
    case UnitAnimationType::RUN_L:
    case UnitAnimationType::JUMP_L:
    case UnitAnimationType::AXE_THROW_PREPARE_L:
    case UnitAnimationType::AXE_THROW_L:
    case UnitAnimationType::AXE_RETURN_L:
    case UnitAnimationType::CROUCH_L:
    case UnitAnimationType::CROUCH_WALK_L:
    case UnitAnimationType::DIE_L:
        return true;
    default:
        return false;
    }
}

// 도끼 던지기 함수
// - Idle 상태에서만 동작하며, 초기 속도를 설정 후 상태를 Thrown으로 변경
void LumberAxeComponent::Throw(const Vector3& initialVelocity)
{
    if (state == AxeState::AXE_IDLE)
    {
        velocity = initialVelocity;
        state = AxeState::AXE_THROWN;
        // 추가: 애니메이션, 사운드 등 처리 가능
    }
}

// 도끼 회수 함수
// - 도끼가 던져진 상태에서 타겟이 설정되어 있을 때 회수 상태로 전환
void LumberAxeComponent::Recall()
{
    if (state == AxeState::AXE_THROWN && target || state == AxeState::AXE_IDLE && target && isLand == true)
    {
        state = AxeState::AXE_RETURNING;
        returnTimer = 0.0f;
        returnStart = owner->GetPosition();

        // 베지어 곡선을 위한 제어점 계산: 시작과 타겟의 중간에 오프셋 추가
        Vector3 tPos = target->GetPosition();
        returnControl = (returnStart + tPos) * 0.5f + Vector3(0, -100, 0);
        // 추가: 회수 시작 시 사운드 등 처리 가능
    }
}

void LumberAxeComponent::Land()
{
    state = AxeState::AXE_IDLE;
    velocity = Vector3::Zero;
    isLand = true;
}

// Idle 상태 업데이트: 타겟 위치로 도끼를 따라가게 하고, 마우스 입력에 따라 준비 상태로 전환
void LumberAxeComponent::UpdateIdleState(float dt)
{
    if (target && isLand == false)
    {
        // 타겟 위치에 오프셋을 더하여 도끼 위치 설정
        Vector3 targetPos = target->GetPosition();
        Vector3 offset(3.0f, -3.5f, 0.0f);
        owner->SetPosition(targetPos + offset);
    }

    // 오른쪽 클릭 시 던지기 준비 상태로 전환
    if (INPUT->MousePressed(1) && isLand == false)
    {
        straightFlightTime = 0.3f; // 직선 이동 시간 재설정
        state = AxeState::AXE_PREPARE;
    }
}

// 준비 상태 업데이트: 왼쪽 클릭 시 도끼 던지기, 오른쪽 버튼 해제 시 Idle 상태 복귀
void LumberAxeComponent::UpdatePrepareState(float dt)
{
    // 타겟 위치에 오프셋을 더하여 도끼 위치 설정
    Vector3 targetPos = target->GetPosition();
    Vector3 offset(10.0f, 0.0f, 0.0f);
    owner->SetPosition(targetPos + offset);

    if (INPUT->MousePressed(0))
    {
        // 마우스 위치 기반 던질 방향 계산
        Vector3 mousePos = INPUT->GetMousePos() + CAMERA->GetPosition();
        Vector3 throwDir = mousePos - owner->GetPosition();

        if (throwDir.Length() > 0.001f) throwDir.Normalize();
        else throwDir = IsFacingLeft(currentAnimation) ? Vector3(-1, 0, 0) : Vector3(1, 0, 0);

        float throwSpeed = 1000.0f;
        velocity = throwDir * throwSpeed;
        state = AxeState::AXE_THROWN;
    }
    else if (INPUT->MouseUp(1))
    {
        // 오른쪽 버튼이 해제되면 준비 취소하고 Idle 상태로 복귀
        state = AxeState::AXE_IDLE;
    }
}

// 던진 상태 업데이트: 초기 직선 비행 후 중력 효과 적용하여 이동, 'E' 키 입력 시 회수 상태로 전환
void LumberAxeComponent::UpdateThrownState(float dt)
{
    Vector3 pos = owner->GetPosition();
    if (straightFlightTime > 0.0f)
    {
        pos += velocity * dt;
        straightFlightTime -= dt;
        if (straightFlightTime < 0.0f) straightFlightTime = 0.0f;
    }
    else
    {
        // 중력 효과 적용 (y축에 가속도)
        velocity.y += -gravity * dt;
        pos += velocity * dt;
    }
    owner->SetPosition(pos);
}

// 회수 상태 업데이트: 베지어 곡선을 따라 도끼를 타겟으로 회수
void LumberAxeComponent::UpdateReturningState(float dt)
{
    returnTimer += dt;
    float t = returnTimer / returnDuration;
    if (t > 1.0f) t = 1.0f;

    Vector3 tPos = target ? target->GetPosition() : Vector3::Zero;
    Vector3 newPos = QuadraticBezier(returnStart, returnControl, tPos, t);
    owner->SetPosition(newPos);

    // 회수가 완료되면 상태를 Idle로 전환하고 위치를 타겟에 맞춤
    if (t >= 1.0f)
    {
        isLand = false;
        state = AxeState::AXE_IDLE;
        velocity = Vector3::Zero;
        owner->SetPosition(tPos);
    }
}

// 던질 방향 계산 함수
// - 마우스 위치와 도끼(오너)의 위치 차이를 기반으로 방향 계산
// - 마우스 위치와 도끼 위치가 가까운 경우 현재 바라보는 방향으로 설정
Vector3 LumberAxeComponent::ComputeThrowDirection()
{
    Vector3 startPos = owner->GetPosition();
    Vector3 mousePos = INPUT->GetMousePos() + CAMERA->GetPosition();
    Vector3 throwDir = mousePos - startPos;

    if (throwDir.Length() > 0.001f) throwDir.Normalize();
    else throwDir = IsFacingLeft(currentAnimation) ? Vector3(-1, 0, 0) : Vector3(1, 0, 0);

    return throwDir;
}

// 궤적 시뮬레이션 함수
// - 초기 위치, 속도, 시간 간격, 시뮬레이션 단계 수를 인자로 받아 도끼의 이동 경로를 점들로 반환
// - straightFlightTime 동안은 중력 효과를 무시하고 직선 이동, 이후 중력 적용
vector<Vector3> LumberAxeComponent::SimulateTrajectory(const Vector3& startPos, const Vector3& initialVel, float dt, int numSteps)
{
    vector<Vector3> points;
    points.reserve(numSteps);

    Vector3 simPos = startPos;
    Vector3 simVel = initialVel;
    float remainingStraightTime = straightFlightTime;

    for (int i = 0; i < numSteps; i++)
    {
        if (remainingStraightTime > 0.0f)
        {
            // 직선 비행: 남은 시간과 dt 중 작은 값을 사용
            float t = min(dt, remainingStraightTime);
            simPos += simVel * t;
            remainingStraightTime -= t;
        }
        else
        {
            // 중력 적용: 속도에 중력 가속도 반영 후 이동
            simVel.y += -gravity * dt;
            simPos += simVel * dt;
        }
        points.push_back(simPos);
    }

    return points;
}

// 궤적 그리기 함수
// - 인자로 받은 점들을 2개씩 연결하여 점선 형태로 렌더링
void LumberAxeComponent::DrawTrajectory(const vector<Vector3>& points, float thickness, const Color& color)
{
    for (size_t i = 0; i < points.size() - 1; i += 2)
    {
        // 카메라 위치를 빼서 화면 좌표로 변환 후 선 그리기
        GIZMO->DrawLine(points[i] - CAMERA->GetPosition(), points[i + 1] - CAMERA->GetPosition(), color, thickness);
    }
}
