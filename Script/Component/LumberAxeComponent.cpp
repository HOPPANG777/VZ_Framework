#include "stdafx.h"
#include "LumberAxeComponent.h"

LumberAxeComponent::LumberAxeComponent(const string& name)
    : Component(name), state(AxeState::AXE_IDLE), velocity(Vector3::Zero)
{

}

void LumberAxeComponent::Update()
{
    float dt = DELTA; // 프레임 델타타임

    // (A) Idle 상태: 도끼를 들고 있다.
    if (state == AxeState::AXE_IDLE)
    {
        // 예를 들어, 마우스 오른쪽 버튼이 눌리면 도끼를 던진다.
        if (INPUT->MousePressed(1))  // 오른쪽 버튼 (혹은 원하는 입력)
        {
            // 던질 방향과 초기 속도는 여기서 정합니다.
            // 예: 플레이어가 바라보는 방향에 따라 (간단하게 오른쪽으로 던진다고 가정)
            velocity = Vector3(500, -300, 0); // x: 500, y: -300 (위로 던지기), z: 0
            state = AxeState::AXE_THROWN;
        }
    }
    // (B) Thrown 상태: 도끼가 던져져 날아가는 중
    else if (state == AxeState::AXE_THROWN)
    {
        // 중력 적용: y축 속도를 증가시킴 (아래로 끌어당김)
        velocity.y += gravity * dt;

        // 현재 도끼 위치 업데이트 (owner는 도끼 오브젝트)
        Vector3 pos = owner->GetPosition();
        pos += velocity * dt;
        owner->SetPosition(pos);

        // 리콜 키 입력이나 일정 거리 도달 시 Returning 상태로 전환
        // 예: E키를 눌렀다면 리콜
        if (INPUT->KeyPressed('E') && target)
        {
            state = AxeState::AXE_RETURNING;
            returnTimer = 0.0f;
            returnStart = pos;
            // 베지어 곡선의 제어점: 현재 위치와 타겟의 중간에서 약간 위쪽
            Vector3 targetPos = target->GetPosition();
            returnControl = (pos + targetPos) * 0.5f + Vector3(0, -100, 0); // 조정 가능
        }
        // 또는 도끼가 너무 멀리 나갔을 경우
        if (pos.x > 1000 || pos.x < -1000)  // 임의의 범위, 필요에 따라 조정
        {
            if (target)
            {
                state = AxeState::AXE_RETURNING;
                returnTimer = 0.0f;
                returnStart = pos;
                Vector3 targetPos = target->GetPosition();
                returnControl = (pos + targetPos) * 0.5f + Vector3(0, -100, 0);
            }
        }
    }
    // (C) Returning 상태: 도끼가 플레이어에게 돌아오는 중
    else if (state == AxeState::AXE_RETURNING)
    {
        returnTimer += dt;
        float t = returnTimer / returnDuration;
        if (t > 1.0f) t = 1.0f;

        Vector3 targetPos = target ? target->GetPosition() : Vector3::Zero;
        Vector3 newPos = QuadraticBezier(returnStart, returnControl, targetPos, t);
        owner->SetPosition(newPos);

        if (t >= 1.0f)
        {
            // 도끼가 플레이어에게 완전히 돌아왔으므로 상태를 Idle로 전환
            state = AxeState::AXE_IDLE;
            velocity = Vector3::Zero;
            // 도끼를 플레이어 위치에 고정
            owner->SetPosition(targetPos);
        }
    }
}

void LumberAxeComponent::Render()
{
    // 도끼의 컴포넌트이므로 별도의 렌더링 로직은 필요없지만,
    // 디버그용으로 경로를 그린다거나 할 수 있음.
}

void LumberAxeComponent::Throw(const Vector3& initialVelocity)
{
    if (state == AxeState::AXE_IDLE)
    {
        velocity = initialVelocity;
        state = AxeState::AXE_THROWN;
    }
}

void LumberAxeComponent::Recall()
{
    if (state == AxeState::AXE_THROWN && target)
    {
        state = AxeState::AXE_RETURNING;
        returnTimer = 0.0f;
        returnStart = owner->GetPosition();
        Vector3 targetPos = target->GetPosition();
        returnControl = (returnStart + targetPos) * 0.5f + Vector3(0, -100, 0);
    }
}

Vector3 LumberAxeComponent::QuadraticBezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t)
{
    float oneMinusT = 1.0f - t;
    return p0 * (oneMinusT * oneMinusT) + p1 * (2 * oneMinusT * t) + p2 * (t * t);
}

// 간단하게 현재 애니메이션이나 도끼 상태를 보고 바라보는 방향 판단 (예제이므로 단순화)
bool LumberAxeComponent::IsFacingLeft(UnitAnimationType currentAnim) const
{
    // 예를 들어, RUN_L, JUMP_L, AXE_THROW_PREPARE_L, etc. 이면 왼쪽으로 판단
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
