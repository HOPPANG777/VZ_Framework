#include "stdafx.h"
#include "PlayerController.h"

// 생성자: 속도, 중력 초기값 등 설정
PlayerController::PlayerController(const string& name, float spd)
    : Component(name),
    speed(spd),
    currentSpeed(spd),
    gravity(150.0f),
    originGravity(150.0f),
    jumpVelocity(0.0f),
    isJump(false),
    isBetween(false),
    isFall(false),
    isCrouch(false),
    isAtking(false),
    isRolling(false),
    isCollision(false),
    isRight(true),
    currentAnimation(UnitAnimationType::IDLE_R),
    isDead(false)
{
}

void PlayerController::Update()
{
    float dt = TIME->GetDeltaTime();
    UpdateMovement(dt);
    UpdateAnimation();
}

void PlayerController::UpdateMovement(float dt)
{
    const std::shared_ptr<Transform>& transform = owner->GetComponent<Transform>("Transform");
    if (!transform) return;
    Vector3 pos = transform->GetPosition();

    // 수평 이동: A, D키에 따라 좌우 이동
    currentSpeed = isCrouch ? speed * 0.25f : speed;
    if (INPUT->KeyPressed('A'))
    {
        isRight = false;
        pos.x -= currentSpeed * dt;
    }
    if (INPUT->KeyPressed('D'))
    {
        isRight = true;
        pos.x += currentSpeed * dt;
    }

    // 점프 시작: Space 키를 누르면 (그리고 현재 점프/낙하 상태가 아니며, 충돌(땅 접촉)이 없는 경우)
    if (INPUT->KeyDown(VK_SPACE) && !isJump && !isBetween && !isFall && !isCollision)
    {
        isJump = true;
        jumpVelocity = speed * 2.0f; // 점프 초기 속도 (조정 가능)
        gravity = originGravity;
    }

    // 수직 이동 및 중력 적용
    if (isJump)
    {
        pos.y += jumpVelocity * TIME->GetDeltaTime();
        jumpVelocity -= gravity * TIME->GetDeltaTime();
        gravity += (gravity * 2.0f) * TIME->GetDeltaTime();

        if (gravity > (speed * 1.5f) && gravity < (speed * 2.0f) || isCollision)
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

        if (gravity > (speed * 2.0f) || isCollision)
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

    // 만약 외부에서 충돌(플랫폼 접촉)이 감지되면 Land()를 호출해 점프 상태 리셋
    if (isCollision)
    {
        Land();
    }

    transform->SetPosition(pos);
}

void PlayerController::UpdateAnimation()
{
    const std::shared_ptr<Animator>& animator = owner->GetComponent<Animator>("Animator");
    if (!animator) return;

    UnitAnimationType newState = currentAnimation;

    // (A) 사망 상태 처리
    if (isDead)
    {
        newState = isRight ? UnitAnimationType::DIE_R : UnitAnimationType::DIE_L;
    }
    // (B) 점프/낙하 상태 처리
    else if (isJump)
    {
        newState = isRight ? UnitAnimationType::JUMP_R : UnitAnimationType::JUMP_L;
    }
    else if (isBetween)
    {
        newState = isRight ? UnitAnimationType::JUMP_BETWEEN_R : UnitAnimationType::JUMP_BETWEEN_L;
    }
    else if (isFall)
    {
        newState = isRight ? UnitAnimationType::JUMP_FALL_R : UnitAnimationType::JUMP_FALL_L;
    }
    // (C) 수평 이동 처리
    else if (INPUT->KeyPressed('A'))
    {
        newState = UnitAnimationType::RUN_L;
    }
    else if (INPUT->KeyPressed('D'))
    {
        newState = UnitAnimationType::RUN_R;
    }
    else
    {
        newState = isRight ? UnitAnimationType::IDLE_R : UnitAnimationType::IDLE_L;
    }

    // (D) 도끼 관련 입력 처리 (예시)
    if (INPUT->MouseDown(2))
    {
        newState = isRight ? UnitAnimationType::AXE_THROW_PREPARE_R : UnitAnimationType::AXE_THROW_PREPARE_L;
    }
    if (INPUT->MousePressed(2) && INPUT->MouseDown(1))
    {
        newState = isRight ? UnitAnimationType::AXE_THROW_R : UnitAnimationType::AXE_THROW_L;
    }
    if (INPUT->KeyDown('E'))
    {
        newState = isRight ? UnitAnimationType::AXE_RETURN_R : UnitAnimationType::AXE_RETURN_L;
    }

    // 상태가 바뀌었으면 애니메이터에 해당 애니메이션 클립 설정
    if (newState != currentAnimation)
    {
        currentAnimation = newState;
        switch (currentAnimation)
        {
        case UnitAnimationType::IDLE_R:
            animator->SetCurrentAnimClip(L"IdleR"); break;
        case UnitAnimationType::IDLE_L:
            animator->SetCurrentAnimClip(L"IdleL"); break;
        case UnitAnimationType::RUN_R:
            animator->SetCurrentAnimClip(L"RunR"); break;
        case UnitAnimationType::RUN_L:
            animator->SetCurrentAnimClip(L"RunL"); break;
        case UnitAnimationType::JUMP_R:
            animator->SetCurrentAnimClip(L"JumpR"); break;
        case UnitAnimationType::JUMP_L:
            animator->SetCurrentAnimClip(L"JumpL"); break;
        case UnitAnimationType::JUMP_BETWEEN_R:
            animator->SetCurrentAnimClip(L"JumpBetweenR"); break;
        case UnitAnimationType::JUMP_BETWEEN_L:
            animator->SetCurrentAnimClip(L"JumpBetweenL"); break;
        case UnitAnimationType::JUMP_FALL_R:
            animator->SetCurrentAnimClip(L"JumpFallR"); break;
        case UnitAnimationType::JUMP_FALL_L:
            animator->SetCurrentAnimClip(L"JumpFallL"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_R:
            animator->SetCurrentAnimClip(L"AxeThrowPrepareR"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_L:
            animator->SetCurrentAnimClip(L"AxeThrowPrepareL"); break;
        case UnitAnimationType::AXE_THROW_R:
            animator->SetCurrentAnimClip(L"AxeThrowR"); break;
        case UnitAnimationType::AXE_THROW_L:
            animator->SetCurrentAnimClip(L"AxeThrowL"); break;
        case UnitAnimationType::AXE_RETURN_R:
            animator->SetCurrentAnimClip(L"AxeReturnR"); break;
        case UnitAnimationType::AXE_RETURN_L:
            animator->SetCurrentAnimClip(L"AxeReturnL"); break;
        default:
            break;
        }
    }
    animator->Update();
}

void PlayerController::Land()
{
    // Land() 호출 시, 점프 관련 상태를 리셋합니다.
    isJump = false;
    isBetween = false;
    isFall = false;
    gravity = originGravity;
    isCollision = false;
}

