#include "stdafx.h"
#include "LumberAxeComponent.h"

LumberAxeComponent::LumberAxeComponent(const string& name)
    : Component(name), state(AxeState::AXE_IDLE), velocity(Vector3::Zero), gravity(980.0f), returnDuration(1.0f), straightFlightTime(0.5f) // 예: 0.3초 동안은 중력 무시(직선 이동)
{

}

void LumberAxeComponent::Update()
{
    float dt = DELTA;
    if (isDead)
    {
        return;
    }

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

void LumberAxeComponent::Render()
{
    // 준비 상태일 때 궤적 미리보기
    if (state == AxeState::AXE_PREPARE)
    {
        // 1) 마우스로부터 던질 방향을 구한다
        Vector3 throwDir = ComputeThrowDirection();

        // 2) 초기 속도 설정
        float throwSpeed = 800.0f;
        Vector3 initialVel = throwDir * throwSpeed;

        //// 3) 궤적 점들을 시뮬레이션
        //vector<Vector3> trajPoints = SimulateTrajectory
        //(
        //    owner->GetPosition(), // 시작점
        //    initialVel,           // 초기 속도
        //    gravity,              // 중력 값
        //    straightFlightTime,   // 직선 비행 시간
        //    DELTA,                 // 시뮬레이션 단위 시간(dtSim)
        //    25                    // 시뮬레이션 점 개수
        //);

        // 3) 실제 UpdateThrownState와 동일한 dt 사용 (예: DELTA)
        // 그리고 충분한 점 수를 사용하여 궤적을 미리 계산
        vector<Vector3> trajPoints = SimulateTrajectory(owner->GetPosition(), initialVel, 0.05f, 30);

        // 4) 점들을 점선으로 그리기
        DrawTrajectory(trajPoints, 5.0f, BLACK);
    }
}

Vector3 LumberAxeComponent::QuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t)
{
    float oneMinusT = 1.0f - t;
    return p0 * (oneMinusT * oneMinusT) + p1 * (2 * oneMinusT * t) + p2 * (t * t);
}

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

void LumberAxeComponent::Throw(const Vector3& initialVelocity)
{
    // Idle 상태에서만 던질 수 있도록 예시
    if (state == AxeState::AXE_IDLE)
    {
        velocity = initialVelocity;  // 인자로 받은 초기 속도 설정
        state = AxeState::AXE_THROWN;
        // 여기서 도끼가 날아가는 애니메이션이나
        // 사운드 재생, 기타 로직을 넣을 수 있음
    }
}

void LumberAxeComponent::Recall()
{
    // 도끼가 날아가고 있을 때만(THROWN) 회수 가능하다고 예시
    if (state == AxeState::AXE_THROWN && target)
    {
        state = AxeState::AXE_RETURNING;
        returnTimer = 0.0f;
        returnStart = owner->GetPosition();

        // 베지어 곡선 제어점을 계산
        Vector3 tPos = target->GetPosition();
        returnControl = (returnStart + tPos) * 0.5f + Vector3(0, -100, 0);

        // 회수 시작 로직 (사운드 재생 등)을 넣을 수도 있음
    }
}

void LumberAxeComponent::UpdateIdleState(float dt)
{
    // target이 설정되어 있으면 target의 위치를 따라감.
    if (target)
    {
        // 예시: target의 위치에 약간의 오프셋(예: 오른쪽 10픽셀)을 더해 도끼 위치를 설정
        Vector3 targetPos = target->GetPosition();
        Vector3 offset(10.0f, 0.0f, 0.0f); // 필요에 따라 조절
        owner->SetPosition(targetPos + offset);
    }

    // Idle 상태 처리: 오른쪽 클릭 → 준비 상태로 전환
    if (INPUT->MousePressed(1))
    {
        state = AxeState::AXE_PREPARE;
    }
}

void LumberAxeComponent::UpdatePrepareState(float dt)
{
    // Prepare 상태: 왼쪽 클릭 시 던지기, 오른쪽 버튼 놓으면 취소
    if (INPUT->MousePressed(0))
    {
        Vector3 mousePos = INPUT->GetMousePos() + CAMERA->GetPosition();
        Vector3 throwDir = mousePos - owner->GetPosition();
        if (throwDir.Length() > 0.001f) throwDir.Normalize();
        else throwDir = IsFacingLeft(currentAnimation) ? Vector3(-1, 0, 0) : Vector3(1, 0, 0);
        float throwSpeed = 800.0f;
        velocity = throwDir * throwSpeed;
        straightFlightTime = 0.3f; // 직선 비행 시간 설정
        state = AxeState::AXE_THROWN;
    }
    else if (!INPUT->MouseDown(1))
    {
        state = AxeState::AXE_IDLE;
    }
}

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
        velocity.y += -gravity * dt;
        pos += velocity * dt;
    }
    owner->SetPosition(pos);
    if (INPUT->KeyPressed('E') && target)
    {
        state = AxeState::AXE_RETURNING;
        returnTimer = 0.0f;
        returnStart = pos;
        Vector3 tPos = target->GetPosition();
        returnControl = (pos + tPos) * 0.5f + Vector3(0, -100, 0);
    }
}

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
        state = AxeState::AXE_IDLE;
        velocity = Vector3::Zero;
        owner->SetPosition(tPos);
    }
}

Vector3 LumberAxeComponent::ComputeThrowDirection()
{
    // 오브젝트(도끼) 위치
    Vector3 startPos = owner->GetPosition();

    // 마우스 월드 위치 (카메라 오프셋 더함)
    Vector3 mousePos = INPUT->GetMousePos() + CAMERA->GetPosition();

    // 방향 벡터
    Vector3 throwDir = mousePos - startPos;

    if (throwDir.Length() > 0.001f) throwDir.Normalize();
    else throwDir = IsFacingLeft(currentAnimation) ? Vector3(-1, 0, 0) : Vector3(1, 0, 0); // 거리가 너무 짧으면, 바라보는 방향으로 대체

    return throwDir;
}

vector<Vector3> LumberAxeComponent::SimulateTrajectory(const Vector3& startPos, const Vector3& initialVel, float dt, int numSteps)
{
    vector<Vector3> points;
    points.reserve(numSteps);

    // 시작 위치와 속도 설정
    Vector3 simPos = startPos;
    Vector3 simVel = initialVel;
    float remainingStraightTime = straightFlightTime; // 직선 비행 시간 (중력 무시 구간)

    for (int i = 0; i < numSteps; i++)
    {
        if (remainingStraightTime > 0.0f)
        {
            // 직선 비행 구간: 중력 효과 없음
            float t = min(dt, remainingStraightTime);
            simPos += simVel * t;
            remainingStraightTime -= t;
        }
        else
        {
            // 중력 적용: y축에 -gravity 가속도 적용
            simVel.y += -gravity * dt;
            simPos += simVel * dt;
        }
        points.push_back(simPos);
    }

    return points;
}

void LumberAxeComponent::DrawTrajectory(const vector<Vector3>& points, float thickness, const Color& color)
{
    // 두 점씩 끊어서 그리면 점선(dotted line) 효과
    // (i += 2) 면 짝수번째-홀수번째만 선
    for (int i = 0; i < (int)points.size() - 1; i += 2)
    {
        GIZMO->DrawLine(points[i], points[i + 1], color, thickness);
    }
}