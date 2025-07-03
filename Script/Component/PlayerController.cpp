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
	isAxeThrow(false),
	isAxePrepare(false),
    isAxePrepareHold(false),
	isAxeReturn(false),
	isClimbing(false)
{
}

void PlayerController::Update()
{
    float dt = Time::Get()->GetDeltaTime();
    UpdateMovement(dt);
    //UpdateAxeInput();
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
    if (Input::Get()->KeyPressed(VK_CONTROL)) isCrouch = true;
    else if (Input::Get()->KeyUp(VK_CONTROL)) isCrouch = false;

    // 수평 이동
    currentSpeed = isCrouch ? speed * 0.25f : speed;
    if (Input::Get()->KeyPressed('A'))
    {
        isRight = false;
        pos.x -= currentSpeed * dt;
    }
    if (Input::Get()->KeyPressed('D'))
    {
        isRight = true;
        pos.x += currentSpeed * dt;
    }

    // 점프 시작
    if (Input::Get()->KeyDown(VK_SPACE) && !isJump && !isBetween && !isFall && isCollision)
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

    // 현재 재생 중인 클립 이름을 가져옴
    wstring currentClip = animator->GetCurrentClipName();

    UnitAnimationType newState = currentAnimation;

    if (isDead)
    {
        newState = isRight ? UnitAnimationType::DIE_R : UnitAnimationType::DIE_L;
    }
    //else if (isAxeReturn)
    //{  
    //    if (animator->IsAnimationFinished()) isAxeReturn = false;
    //    newState = isRight ? UnitAnimationType::AXE_RETURN_R : UnitAnimationType::AXE_RETURN_L;
    //}
    //else if (isAxeThrow)
    //{
    //    if (animator->IsAnimationFinished()) isAxeThrow = false;
    //    newState = isRight ? UnitAnimationType::AXE_THROW_R : UnitAnimationType::AXE_THROW_L;
    //}
    //else if (isAxePrepareHold)
    //{
    //    newState = isRight ? UnitAnimationType::AXE_THROW_PREPARE_HOLD_R : UnitAnimationType::AXE_THROW_PREPARE_HOLD_L;
    //}
    //else if (isAxePrepare)
    //{
    //    newState = isRight ? UnitAnimationType::AXE_THROW_PREPARE_R : UnitAnimationType::AXE_THROW_PREPARE_L;
    //}
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
    else if (isCrouch && Input::Get()->KeyPressed('A'))
    {
        newState = UnitAnimationType::CROUCH_WALK_L;
    }
    else if (isCrouch && Input::Get()->KeyPressed('D'))
    {
        newState = UnitAnimationType::CROUCH_WALK_R;
    }
    else if (isCrouch && !Input::Get()->KeyPressed('A') && !Input::Get()->KeyPressed('D') && !Input::Get()->KeyPressed(VK_SPACE) && !Input::Get()->MouseDown(1) && !Input::Get()->KeyDown('E'))
    {
        newState = isRight ? UnitAnimationType::CROUCH_HOLD_R : UnitAnimationType::CROUCH_HOLD_L;
    }
    else if (isCrouch)
    {
        newState = isRight ? UnitAnimationType::CROUCH_R : UnitAnimationType::CROUCH_L;
    }
    else if (Input::Get()->KeyPressed('A'))
    {
        newState = UnitAnimationType::RUN_L;
    }
    else if (Input::Get()->KeyPressed('D'))
    {
        newState = UnitAnimationType::RUN_R;
    }
    else if (Input::Get()->KeyDown(VK_SPACE) && !isJump && !isBetween && !isFall && isCollision)
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
        case UnitAnimationType::IDLE_R:                     animator->SetCurrentAnimClip(L"IdleR"); break;
        case UnitAnimationType::IDLE_L:                     animator->SetCurrentAnimClip(L"IdleL"); break;
        case UnitAnimationType::RUN_R:                      animator->SetCurrentAnimClip(L"RunR"); break;
        case UnitAnimationType::RUN_L:                      animator->SetCurrentAnimClip(L"RunL"); break;
        case UnitAnimationType::CROUCH_R:                   animator->SetCurrentAnimClip(L"CrouchR"); break;
        case UnitAnimationType::CROUCH_L:                   animator->SetCurrentAnimClip(L"CrouchL"); break;
        case UnitAnimationType::CROUCH_WALK_R:              animator->SetCurrentAnimClip(L"CrouchWalkR"); break;
        case UnitAnimationType::CROUCH_WALK_L:              animator->SetCurrentAnimClip(L"CrouchWalkL"); break;
        case UnitAnimationType::CROUCH_HOLD_R:              animator->SetCurrentAnimClip(L"CrouchHoldR"); break;
        case UnitAnimationType::CROUCH_HOLD_L:              animator->SetCurrentAnimClip(L"CrouchHoldL"); break;
        case UnitAnimationType::JUMP_R:                     animator->SetCurrentAnimClip(L"JumpR"); break;
        case UnitAnimationType::JUMP_L:                     animator->SetCurrentAnimClip(L"JumpL"); break;
        case UnitAnimationType::JUMP_BETWEEN_R:             animator->SetCurrentAnimClip(L"JumpBetweenR"); break;
        case UnitAnimationType::JUMP_BETWEEN_L:             animator->SetCurrentAnimClip(L"JumpBetweenL"); break;
        case UnitAnimationType::JUMP_FALL_R:                animator->SetCurrentAnimClip(L"JumpFallR"); break;
        case UnitAnimationType::JUMP_FALL_L:                animator->SetCurrentAnimClip(L"JumpFallL"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_R:        animator->SetCurrentAnimClip(L"AxePrepareR"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_L:        animator->SetCurrentAnimClip(L"AxePrepareL"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_HOLD_R:   animator->SetCurrentAnimClip(L"AxePrepareHoldR"); break;
        case UnitAnimationType::AXE_THROW_PREPARE_HOLD_L:   animator->SetCurrentAnimClip(L"AxePrepareHoldL"); break;
        case UnitAnimationType::AXE_THROW_R:                animator->SetCurrentAnimClip(L"AxeThrowR"); animator->SetLoop(false); break;
        case UnitAnimationType::AXE_THROW_L:                animator->SetCurrentAnimClip(L"AxeThrowL"); animator->SetLoop(false); break;
        case UnitAnimationType::AXE_RETURN_R:               animator->SetCurrentAnimClip(L"AxeReturnR"); animator->SetLoop(false); break;
        case UnitAnimationType::AXE_RETURN_L:               animator->SetCurrentAnimClip(L"AxeReturnL"); animator->SetLoop(false); break;
        case UnitAnimationType::DIE_R:                      animator->SetCurrentAnimClip(L"DieR"); break;
        case UnitAnimationType::DIE_L:                      animator->SetCurrentAnimClip(L"DieL"); break;
        default: break;
        }
    }

    animator->Update();
}

void PlayerController::UpdateAxeInput()
{
    //——————
   // 1) 오른쪽 버튼 누른 첫 프레임 → Prepare 진입
   //    (다른 Axe 동작 중이 아닐 때만)
   //——————
    if (Input::Get()->MouseDown(1))
    {
        if (!isAxePrepare && !isAxePrepareHold && !isAxeThrow && !isAxeReturn)
        {
            isAxePrepare = true;
        }
    }

    //——————
    // 2) Prepare 상태에서 버튼을 계속 누르고 있으면 → PrepareHold 전환
    //    (MousePressed는 “버튼이 눌린 상태 유지”)
    //——————
    if (isAxePrepare)
    {
        isAxePrepareHold = true;
        isAxePrepare = false;
    }

    //——————
    // 3) 오른쪽 버튼을 뗀 순간 → Prepare / PrepareHold 초기화
    //——————
    if (Input::Get()->MouseUp(1))
    {
        if (isAxePrepare || isAxePrepareHold)
        {
            isAxePrepare = false;
            isAxePrepareHold = false;
        }
    }

    //——————
    // 4) PrepareHold 상태에서 왼쪽 버튼 클릭 → Throw 진입
    //    (isAxeThrow 플래그만 설정, 다음 UpdateAnimation에서 처리)
    //——————
    if (isAxePrepareHold && Input::Get()->MouseDown(0) && !isAxeThrow && !isAxeReturn)        // 이미 Throw/Return 중이면 무시
    {
        isAxeThrow = true;
        isAxePrepareHold = false;
    }

    //——————
    // 5) E 키를 누르면 → Return 진입
    //    (다른 Axe 동작 중이 아닐 때만)
    //——————
    if (Input::Get()->KeyDown('E') && !isAxePrepare && !isAxePrepareHold && !isAxeThrow && !isAxeReturn)
    {
        isAxeReturn = true;
    }
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

