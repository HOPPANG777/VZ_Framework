#include "Framework.h"
#include "PlayerMove.h"

void PlayerMove::Update()
{
    auto transform = owner->GetComponent<Transform>("Transform");
    if (!transform) return;

    Vector3 pos = transform->GetPosition();

    // [숙이기 상태 업데이트]
    if (INPUT->KeyPressed(VK_CONTROL)) isCrouch = true;
    else if (INPUT->KeyUp(VK_CONTROL)) isCrouch = false;

    // [좌우 이동 처리] - 공중에서도 좌우 이동 가능
    currentSpeed = isCrouch ? speed * 0.25f : speed;
    if (INPUT->KeyPressed('A'))
    {
        isRight = false;
        pos.x -= currentSpeed * DELTA;
    }
    if (INPUT->KeyPressed('D'))
    {
        isRight = true;
        pos.x += currentSpeed * DELTA;
    }

    // [점프 처리]
    if (INPUT->KeyDown(VK_SPACE) && !isJump && !isBetween && !isFall)
    {
        isJump = true;
        jumpVelocity = speed * 2.0f;  // 점프 시작 속도
        gravity = originGravity;           // 중력 초기화
    }

    // 수직 이동 및 중력 적용
    if (isJump)
    {
        pos.y += jumpVelocity * DELTA;
        jumpVelocity -= gravity * DELTA;
        gravity += (gravity * 2.0f) * DELTA;

        // 특정 중력 범위에 도달하면 점프 종료 후 중간 상태로 전환
        if (gravity > (speed * 1.5f) && gravity < (speed * 2.0f))
        {
            isJump = false;
            isBetween = true;
        }
    }
    else if (isBetween)
    {
        pos.y += jumpVelocity * DELTA;
        pos.y -= gravity * DELTA;
        gravity += (gravity * 2.0f) * DELTA;

        if (gravity > (speed * 2.0f))
        {
            isBetween = false;
            isFall = true;
        }
    }
    else if (isFall)
    {
        pos.y -= gravity * DELTA;
        gravity += (gravity * 2.0f) * DELTA;
    }
    else
    {
        // 땅에 있거나 아무 상태에도 있지 않을 때도 중력 적용
        pos.y -= gravity * DELTA;
        gravity += (gravity * 2.0f) * DELTA;
    }

    transform->SetPosition(pos);

    // 만약 충돌(땅과의 접촉)이 감지되었다면, y 위치를 고정하고 Land() 호출
    if (isCollision)
    {
        // 예: 땅의 y값을 0으로 가정 (또는 GetGroundLevel()을 호출)
        //pos.y = 0.0f;
        //transform->SetPosition(pos);
        Land();
    }

    if (INPUT->KeyUp('D')) rightMove = false;
}

void PlayerMove::Render()
{
    // 별도 렌더링이 필요하지 않다면 빈 구현
}

void PlayerMove::Land()
{
    isFall = false;
    gravity = originGravity;
    // 충돌 상태 해제
    isCollision = false;
}
