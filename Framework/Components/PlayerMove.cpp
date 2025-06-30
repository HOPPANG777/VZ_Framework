#include "Framework.h"
#include "PlayerMove.h"
#include "Systems/Input.h"
#include "Systems/Time.h"
#include "Components/Transform.h"

PlayerMove::PlayerMove(const string& name, float spd)
    : Component(name), speed(spd), currentSpeed(spd),
      gravity(150.0f), originGravity(150.0f), jumpVelocity(0.0f),
      isJump(false), isBetween(false), isFall(false), isCrouch(false),
      rightMove(false), isRight(true), isAtking(false), isRolling(false),
      isCollision(false)
{
}

void PlayerMove::Update()
{
    const auto& transform = owner->GetComponent<Transform>("Transform");
    if (!transform) return;

    Vector3 pos = transform->GetPosition();

    // 숙이기 상태 업데이트
    if (INPUT->KeyPressed(VK_CONTROL)) isCrouch = true;
    else if (INPUT->KeyUp(VK_CONTROL)) isCrouch = false;

    // 좌우 이동 처리
    currentSpeed = isCrouch ? speed * 0.25f : speed;
    if (INPUT->KeyPressed('A'))
    {
        isRight = false;
        pos.x -= currentSpeed * TIME->GetDeltaTime();
    }
    if (INPUT->KeyPressed('D'))
    {
        isRight = true;
        pos.x += currentSpeed * TIME->GetDeltaTime();
    }

    // 점프 시작
    if (INPUT->KeyDown(VK_SPACE) && !isJump && !isBetween && !isFall)
    {
        isJump = true;
        jumpVelocity = speed * 2.0f;
        gravity = originGravity;
    }

    // 수직 이동 및 중력 적용
    if (isJump)
    {
        pos.y += jumpVelocity * TIME->GetDeltaTime();
        jumpVelocity -= gravity * TIME->GetDeltaTime();
        gravity += (gravity * 2.0f) * TIME->GetDeltaTime();

        if (gravity > (speed * 1.5f) && gravity < (speed * 2.0f))
        {
            isJump = false;
            isBetween = true;
        }
    }
    else if (isBetween)
    {
        pos.y += jumpVelocity * TIME->GetDeltaTime();
        pos.y -= gravity * TIME->GetDeltaTime();
        gravity += (gravity * 2.0f) * TIME->GetDeltaTime();

        if (gravity > (speed * 2.0f))
        {
            isBetween = false;
            isFall = true;
        }
    }
    else if (isFall)
    {
        pos.y -= gravity * TIME->GetDeltaTime();
        gravity += (gravity * 2.0f) * TIME->GetDeltaTime();
    }
    else
    {
        // 바닥에 있는 상태: 중력이 계속 작용하지 않도록 (땅에 고정)
        // 이 경우 충돌로부터 Land()를 호출하여 상태를 리셋함.
        // 땅에 있거나 아무 상태에도 있지 않을 때도 중력 적용
        //pos.y -= gravity * DELTA;
        //gravity += (gravity * 2.0f) * DELTA;
    }

    transform->SetPosition(pos);

    // 외부 충돌(예, FloorCollision 등)에서 isCollision이 true로 설정되면 Land() 호출
    if (isCollision)
    {
        Land();
    }
}

void PlayerMove::Render()
{
    // 이동 컴포넌트는 별도 렌더링이 필요 없음.
}

void PlayerMove::Land()
{
    // 땅과 충돌하여 Land()가 호출될 때
    isFall = false;
    isJump = false;
    isBetween = false;
    gravity = originGravity;
    isCollision = false;
}

