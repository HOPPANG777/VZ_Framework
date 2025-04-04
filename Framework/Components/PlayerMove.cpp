#include "Framework.h"
#include "PlayerMove.h"

void PlayerMove::Update()
{
    auto transform = owner->GetComponent<Transform>("Transform");
    if (!transform) return;

    Vector3 pos = transform->GetPosition();

    // [숙이기 상태 업데이트]
    if (INPUT->KeyPressed(VK_CONTROL)) mCrouch = true;
    else if (INPUT->KeyUp(VK_CONTROL)) mCrouch = false;

    // [좌우 이동 처리] - 공중에서도 좌우 이동 가능
    float currentSpeed = mCrouch ? mSpd * 0.25f : mSpd;
    if (INPUT->KeyPressed('A'))
    {
        mRight = false;
        pos.x -= currentSpeed * DELTA;
    }
    if (INPUT->KeyPressed('D'))
    {
        mRight = true;
        pos.x += currentSpeed * DELTA;
    }

    // [점프 처리]
    if (INPUT->KeyDown(VK_SPACE) && !mJump && !mBetween && !mFall)
    {
        mJump = true;
        jumpVelocity = mSpd * 2.0f;  // 점프 시작 속도
        mGravity = mGInit;           // 중력 초기화
    }

    // 수직 이동 및 중력 적용
    if (mJump)
    {
        pos.y += jumpVelocity * DELTA;
        jumpVelocity -= mGravity * DELTA;
        mGravity += (mGravity * 2.0f) * DELTA;

        // 특정 중력 범위에 도달하면 점프 종료 후 중간 상태로 전환
        if (mGravity > (mSpd * 1.5f) && mGravity < (mSpd * 2.0f))
        {
            mJump = false;
            mBetween = true;
        }
    }
    else if (mBetween)
    {
        pos.y += jumpVelocity * DELTA;
        pos.y -= mGravity * DELTA;
        mGravity += (mGravity * 2.0f) * DELTA;

        if (mGravity > (mSpd * 2.0f))
        {
            mBetween = false;
            mFall = true;
        }
    }
    else if (mFall)
    {
        pos.y -= mGravity * DELTA;
        mGravity += (mGravity * 2.0f) * DELTA;
    }
    else
    {
        // 땅에 있거나 아무 상태에도 있지 않을 때도 중력 적용
        pos.y -= mGravity * DELTA;
        mGravity += (mGravity * 2.0f) * DELTA;
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

    if (INPUT->KeyUp('D')) moveR = false;
}

void PlayerMove::Render()
{
    // 별도 렌더링이 필요하지 않다면 빈 구현
}

void PlayerMove::Land()
{
    mFall = false;
    mGravity = mGInit;
    // 충돌 상태 해제
    isCollision = false;
}
