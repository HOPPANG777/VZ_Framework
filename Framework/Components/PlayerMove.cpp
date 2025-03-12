#include "Framework.h"
#include "PlayerMove.h"

void PlayerMove::Update()
{
    auto transform = owner->GetComponent<Transform>("Transform");
    if (!transform) return;

    Vector3 pos = transform->GetPosition();

    // [숙이기 상태 업데이트]
    if (INPUT->KeyPressed(VK_CONTROL))
        mCrouch = true;
    else if (INPUT->KeyUp(VK_CONTROL))
        mCrouch = false;

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

    if (mJump)
    {
        pos.y += jumpVelocity * DELTA;
        jumpVelocity -= mGravity * DELTA;
        mGravity += (mGravity * 2.0f) * DELTA;

        // 특정 중력 범위에 도달하면 점프 단계를 종료하고 중간 상태로 전환
        if (mGravity > (mSpd * 1.5f) && mGravity < (mSpd * 2.0f))
        {
            mJump = false;
            mBetween = true;
        }
    }
    else if (mBetween)
    {
        // 최고점 이후의 잠깐의 정점(속도 0에 가까워지는 시점)
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

    transform->SetPosition(pos);

    if (INPUT->KeyUp('D')) moveR = false;
}

void PlayerMove::Render()
{
}

void PlayerMove::Land()
{
    mFall = false;
    mGravity = mGInit;
}

