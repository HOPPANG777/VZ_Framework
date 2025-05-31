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
    isDead(false),
    isAxeing(false),
	isAxeThrow(false),
	isAxePrepare(false),
	isAxeReturn(false),
	isClimbing(false)
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
    auto transform = owner->GetComponent<Transform>("Transform");
    if (!transform) return;

    // (1) 현재 위치 읽어오기
    Vector3 cur = transform->GetPosition();
    Vector3 pos = cur;
    
    // 숙이기 상태 업데이트
    if (INPUT->KeyPressed(VK_CONTROL)) isCrouch = true;
    else if (INPUT->KeyUp(VK_CONTROL)) isCrouch = false;

    // 수평 이동
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

    // 점프 시작
    if (INPUT->KeyDown(VK_SPACE) && !isJump && !isBetween && !isFall && isCollision)
    {
        isJump = true;
        jumpVelocity = speed * 2.0f;
        gravity = originGravity;
        isCollision = false;
    }

    // 수직 이동/중력 적용
    if (isJump)
    {
        pos.y += jumpVelocity * dt;
        jumpVelocity -= gravity * dt;
        gravity += (gravity * 2.0f) * dt;

        if (gravity > speed * 1.5f && gravity < speed * 2.0f)
        {
            isJump = false;
            isBetween = true;
        }
    }
    else if (isBetween)
    {
        pos.y += jumpVelocity * dt - gravity * dt;
        gravity += (gravity * 2.0f) * dt;

        if (gravity > speed * 2.0f)
        {
            isBetween = false;
            isFall = true;
        }
    }
    else if (isFall)
    {
        pos.y -= gravity * dt;
        gravity += (gravity * 2.0f) * dt;
    }
    else
    {
        pos.y = cur.y;
    }

    // 착지 시점: 실제 낙하 중일 때만 Land() 호출
    //if (isCollision && isFall)
    {
        //Land();
    }

    
    //Vector3 newPos = cur;
    //newPos.x = pos.x;                     // 언제나 X 갱신
    //if (!isCollision) newPos.y = pos.y;   // 땅에 닿아 있을 때는 Y 덮어쓰지 않음 → 충돌 보정 유지 
    //transform->SetPosition(newPos);

    transform->SetPosition(pos);

    //transform->SetPosition({ pos.x,(isCollision ? cur.y : pos.y),0 });
}

void PlayerController::UpdateAnimation()
{
    auto animator = owner->GetComponent<Animator>("Animator");
    if (!animator) return;

    if ((isAxePrepare || isAxeThrow || isAxeReturn) && !animator->IsAnimationFinished())
    {
        animator->Update();
        return;
    }

    // (b) 던지기 애니메이션이 끝났다면 isAxeThrow 비활성화 (던진 후 대기 상태로 전환)
    if (isAxeThrow && animator->IsAnimationFinished())
    {
        isAxeThrow = false;
        // 던진 상태에서는 별도 처리 없이 isAxeing만 유지 (나중에 'E' 키로 반환 시작)
    }

    // (c) 반환 애니메이션이 끝났다면 모든 도끼 액션 플래그 해제
    if (isAxeReturn && animator->IsAnimationFinished())
    {
        isAxeReturn = false;
        isAxeing = false;
    }

    // (d) 아직 도끼 액션을 시작하지 않은 상태에서 마우스 오른쪽 클릭 → 준비 상태로 진입
    if (!isAxeing && INPUT->MouseDown(1))
    {
        isAxeing = true;
        isAxePrepare = true;
        isAxeThrow = false;
        isAxeReturn = false;
    }

    UnitAnimationType newState = currentAnimation;

    if (isAxeing && INPUT->MouseDown(1)) 
    {
        newState = isRight ? UnitAnimationType::AXE_THROW_PREPARE_R : UnitAnimationType::AXE_THROW_PREPARE_L;
    }
    else if (isAxeing && INPUT->MousePressed(1) && INPUT->MouseDown(0)) 
    {
        newState = isRight ? UnitAnimationType::AXE_THROW_R : UnitAnimationType::AXE_THROW_L;
    }
    else if (isAxeing && INPUT->MousePressed(1)) 
    {
        newState = isRight ? UnitAnimationType::AXE_THROW_PREPARE_HOLD_R : UnitAnimationType::AXE_THROW_PREPARE_HOLD_L;
    }
    else if (isAxeing && INPUT->KeyDown('E')) 
    {
        newState = isRight ? UnitAnimationType::AXE_RETURN_R : UnitAnimationType::AXE_RETURN_L;
        isAxeing = false; // 반환 시작 시 도끼 액션 종료
    }
    else if (isDead) 
    {
        newState = isRight ? UnitAnimationType::DIE_R : UnitAnimationType::DIE_L;
    }
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
    else if (isCrouch && INPUT->KeyPressed('A')) 
    {
        newState = UnitAnimationType::CROUCH_WALK_L;
    }
    else if (isCrouch && INPUT->KeyPressed('D')) 
    {
        newState = UnitAnimationType::CROUCH_WALK_R;
    }
    else if (isCrouch && !INPUT->KeyPressed('A') && !INPUT->KeyPressed('D') && !INPUT->KeyPressed(VK_SPACE) && !INPUT->MouseDown(1) && !INPUT->KeyDown('E')) 
    {
        newState = isRight ? UnitAnimationType::CROUCH_HOLD_R : UnitAnimationType::CROUCH_HOLD_L;
    }
    else if (isCrouch) 
    {
        newState = isRight ? UnitAnimationType::CROUCH_R : UnitAnimationType::CROUCH_L;
    }
    else if (INPUT->KeyPressed('A')) 
    {
        newState = UnitAnimationType::RUN_L;
    }
    else if (INPUT->KeyPressed('D')) 
    {
        newState = UnitAnimationType::RUN_R;
    }
    else if (INPUT->KeyDown(VK_SPACE) && !isJump && !isBetween && !isFall && isCollision) 
    {
        newState = isRight ? UnitAnimationType::JUMP_R : UnitAnimationType::JUMP_L;
    }
    else 
    {
        newState = isRight ? UnitAnimationType::IDLE_R : UnitAnimationType::IDLE_L;
    }

    if (newState != currentAnimation) 
    {
        currentAnimation = newState;
        switch (currentAnimation) 
        {
        case UnitAnimationType::IDLE_R:              animator->SetCurrentAnimClip(L"IdleR"); break;
        case UnitAnimationType::IDLE_L:              animator->SetCurrentAnimClip(L"IdleL"); break;
        case UnitAnimationType::RUN_R:               animator->SetCurrentAnimClip(L"RunR"); break;
        case UnitAnimationType::RUN_L:               animator->SetCurrentAnimClip(L"RunL"); break;
        case UnitAnimationType::CROUCH_R:            animator->SetCurrentAnimClip(L"CrouchR"); break;
        case UnitAnimationType::CROUCH_L:            animator->SetCurrentAnimClip(L"CrouchL"); break;
        case UnitAnimationType::CROUCH_WALK_R:       animator->SetCurrentAnimClip(L"CrouchWalkR"); break;
        case UnitAnimationType::CROUCH_WALK_L:       animator->SetCurrentAnimClip(L"CrouchWalkL"); break;
        case UnitAnimationType::CROUCH_HOLD_R:       animator->SetCurrentAnimClip(L"CrouchHoldR"); break;
        case UnitAnimationType::CROUCH_HOLD_L:       animator->SetCurrentAnimClip(L"CrouchHoldL"); break;
        case UnitAnimationType::JUMP_R:              animator->SetCurrentAnimClip(L"JumpR"); break;
        case UnitAnimationType::JUMP_L:              animator->SetCurrentAnimClip(L"JumpL"); break;
        case UnitAnimationType::JUMP_BETWEEN_R:      animator->SetCurrentAnimClip(L"JumpBetweenR"); break;
        case UnitAnimationType::JUMP_BETWEEN_L:      animator->SetCurrentAnimClip(L"JumpBetweenL"); break;
        case UnitAnimationType::JUMP_FALL_R:         animator->SetCurrentAnimClip(L"JumpFallR"); break;
        case UnitAnimationType::JUMP_FALL_L:         animator->SetCurrentAnimClip(L"JumpFallL"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_R: animator->SetCurrentAnimClip(L"AxePrepareR"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_L: animator->SetCurrentAnimClip(L"AxePrepareL"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_HOLD_R: animator->SetCurrentAnimClip(L"AxePrepareHoldR"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_HOLD_L: animator->SetCurrentAnimClip(L"AxePrepareHoldL"); break;
        case UnitAnimationType::AXE_THROW_R:         animator->SetCurrentAnimClip(L"AxeThrowR"); break;
        case UnitAnimationType::AXE_THROW_L:         animator->SetCurrentAnimClip(L"AxeThrowL"); break;
        case UnitAnimationType::AXE_RETURN_R:        animator->SetCurrentAnimClip(L"AxeReturnR"); break;
        case UnitAnimationType::AXE_RETURN_L:        animator->SetCurrentAnimClip(L"AxeReturnL"); break;
        case UnitAnimationType::DIE_R:               animator->SetCurrentAnimClip(L"DieR"); break;
        case UnitAnimationType::DIE_L:               animator->SetCurrentAnimClip(L"DieL"); break;
        default: break;
        }
    }

    animator->Update();
}

void PlayerController::Land()
{
    // 한번만 호출되도록 조건을 걸었으므로, 착지 시 완전히 리셋
    isJump = false;
    isBetween = false;
    isFall = false;
    gravity = originGravity;
    isCollision = false;
}

