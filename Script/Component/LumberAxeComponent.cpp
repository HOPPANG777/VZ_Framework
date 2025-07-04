#include "stdafx.h"
#include "LumberAxeComponent.h"

// 생성자: 초기 상태, 속도, 중력, 직선 이동 시간 설정
LumberAxeComponent::LumberAxeComponent(const std::string& name)
    : Component(name), state(AxeState::AXE_IDLE_R), velocity(Vector3::Zero), gravity(980.0f), returnDuration(0.5f), straightFlightTime(0.3f)
{

}

// 매 프레임 호출되는 업데이트 함수
void LumberAxeComponent::Update()
{
    double dt = Time::Get()->GetDeltaTime();
    if (isDead) return;

    switch (state)
    {
    case AxeState::AXE_IDLE_R:
    case AxeState::AXE_IDLE_L:
        UpdateIdleState(dt);
        break;
    case AxeState::AXE_PREPARE_R:
    case AxeState::AXE_PREPARE_L:
        UpdatePrepareState(dt);
        break;
    case AxeState::AXE_THROWN_R:
    case AxeState::AXE_THROWN_L:
        UpdateThrownState(dt);
        break;
    case AxeState::AXE_RETURNING_R:
    case AxeState::AXE_RETURNING_L:
        UpdateReturningState(dt);
        break;
    default:
        break;
    }
}

// Render: 준비 상태일 때 도끼 궤적 미리보기 그리기
void LumberAxeComponent::Render()
{
    if (state == AxeState::AXE_PREPARE_R || state == AxeState::AXE_PREPARE_L)
    {
        Vector3 throwDir = ComputeThrowDirection();
        float throwSpeed = 1000.0f;
        Vector3 initialVel = throwDir * throwSpeed;
        std::vector<Vector3> trajPoints = SimulateTrajectory(owner->GetPosition(), initialVel, 0.02f, 64);
        DrawTrajectory(trajPoints, 3.0f, WHITE);
    }
}

// 2차 베지어 곡선 계산 함수
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
void LumberAxeComponent::Throw(const Vector3& initialVelocity)
{
    if (state == AxeState::AXE_IDLE_R || state == AxeState::AXE_IDLE_L)
    {
        velocity = initialVelocity;
        // 선택: 현재 플레이어 애니메이션 상태에 따라 던진 상태 결정
        state = IsFacingLeft(currentAnimation) ? AxeState::AXE_THROWN_L : AxeState::AXE_THROWN_R;
        // 추가: 애니메이션, 사운드 등 처리 가능
    }
}

// 도끼 회수 함수
void LumberAxeComponent::Recall()
{
    // 조건: 도끼가 던져진 상태일 때 회수
    if ((state == AxeState::AXE_THROWN_R || state == AxeState::AXE_THROWN_L || state == AxeState::AXE_IDLE_R && target && isLand == true || state == AxeState::AXE_IDLE_L && target && isLand == true) && target)
    {
        state = IsFacingLeft(currentAnimation) ? AxeState::AXE_RETURNING_L : AxeState::AXE_RETURNING_R;
        returnTimer = 0.0f;
        returnStart = owner->GetPosition();
        Vector3 tPos = target->GetPosition();
        returnControl = (returnStart + tPos) * 0.5f + Vector3(0, -100, 0);
    }
}

void LumberAxeComponent::Land()
{
    state = IsFacingLeft(currentAnimation) ? AxeState::AXE_IDLE_L : AxeState::AXE_IDLE_R;
    velocity = Vector3::Zero;
    isLand = true;
}

// Idle 상태 업데이트: 타겟 위치에 따라 도끼 위치 설정 및 준비 상태 전환
void LumberAxeComponent::UpdateIdleState(float dt)
{
    if (target && !isLand)
    {
        Vector3 targetPos = target->GetPosition();
        Vector3 offset(3.0f, -3.5f, 0.0f);
        owner->SetPosition(targetPos + offset);
        state = IsFacingLeft(currentAnimation) ? AxeState::AXE_IDLE_L : AxeState::AXE_IDLE_R;
    }

    if (Input::Get()->MousePressed(1) && !isLand)
    {
        straightFlightTime = 0.3f;
        state = IsFacingLeft(currentAnimation) ? AxeState::AXE_PREPARE_L : AxeState::AXE_PREPARE_R;
    }
}

// 준비 상태 업데이트: 왼쪽 클릭 시 던지기 실행, 오른쪽 버튼 해제 시 Idle로 복귀
void LumberAxeComponent::UpdatePrepareState(float dt)
{
    Vector3 targetPos = target->GetPosition();
    Vector3 offset(10.0f, 0.0f, 0.0f);
    owner->SetPosition(targetPos + offset);

    if (Input::Get()->MousePressed(0))
    {
        Vector3 mousePos = Input::Get()->GetMousePos() + Camera::Get()->GetPosition();
        Vector3 throwDir = mousePos - owner->GetPosition();
        if (throwDir.Length() > 0.001f) throwDir.Normalize();
        else throwDir = IsFacingLeft(currentAnimation) ? Vector3(-1, 0, 0) : Vector3(1, 0, 0);
        float throwSpeed = 1000.0f;
        velocity = throwDir * throwSpeed;
        state = IsFacingLeft(currentAnimation) ? AxeState::AXE_THROWN_L : AxeState::AXE_THROWN_R;
    }
    else if (Input::Get()->MouseUp(1))
    {
        state = IsFacingLeft(currentAnimation) ? AxeState::AXE_IDLE_L : AxeState::AXE_IDLE_R;
    }
}

// 던진 상태 업데이트: 직선 비행 후 중력 적용
void LumberAxeComponent::UpdateThrownState(float dt)
{
    Vector3 pos = owner->GetPosition();
    if (straightFlightTime > 0.0f)
    {
        pos += velocity * dt;
        straightFlightTime -= dt;
        if (straightFlightTime < 0.0f)
            straightFlightTime = 0.0f;
    }
    else
    {
        velocity.y += -gravity * dt;
        pos += velocity * dt;
    }
    owner->SetPosition(pos);
}

// 회수 상태 업데이트: 베지어 곡선을 따라 회수
void LumberAxeComponent::UpdateReturningState(float dt)
{
    returnTimer += dt;
    float t = returnTimer / returnDuration;
    if (t > 1.0f) t = 1.0f;

    Vector3 tPos = target ? target->GetPosition() : Vector3::Zero;
    Vector3 newPos = QuadraticBezier(returnStart, returnControl, tPos, t);
    owner->SetPosition(newPos);

    if (t >= 1.0f)
    {
        isLand = false;
        state = IsFacingLeft(currentAnimation) ? AxeState::AXE_IDLE_L : AxeState::AXE_IDLE_R;
        velocity = Vector3::Zero;
        owner->SetPosition(tPos);
    }
}

// 던질 방향 계산 함수: 마우스와 도끼 위치 차이
Vector3 LumberAxeComponent::ComputeThrowDirection()
{
    Vector3 startPos = owner->GetPosition();
    Vector3 mousePos = Input::Get()->GetMousePos() + Camera::Get()->GetPosition();
    Vector3 throwDir = mousePos - startPos;

    if (throwDir.Length() > 0.001f) throwDir.Normalize();
    else throwDir = IsFacingLeft(currentAnimation) ? Vector3(-1, 0, 0) : Vector3(1, 0, 0);

    return throwDir;
}

// 궤적 시뮬레이션 함수
std::vector<Vector3> LumberAxeComponent::SimulateTrajectory(const Vector3& startPos, const Vector3& initialVel, float dt, int numSteps)
{
    std::vector<Vector3> points;
    points.reserve(numSteps);

    Vector3 simPos = startPos;
    Vector3 simVel = initialVel;
    float remainingStraightTime = straightFlightTime;

    for (int i = 0; i < numSteps; i++)
    {
        if (remainingStraightTime > 0.0f)
        {
            float t = min(dt, remainingStraightTime);
            simPos += simVel * t;
            remainingStraightTime -= t;
        }
        else
        {
            simVel.y += -gravity * dt;
            simPos += simVel * dt;
        }
        points.push_back(simPos);
    }

    return points;
}

// 궤적 그리기 함수
void LumberAxeComponent::DrawTrajectory(const std::vector<Vector3>& points, float thickness, const Color& color)
{
    for (size_t i = 0; i < points.size() - 1; i += 2)
    {
        Gizmo::Get()->DrawLine(points[i] - Camera::Get()->GetPosition(), points[i + 1] - Camera::Get()->GetPosition(), color, thickness);
    }
}
