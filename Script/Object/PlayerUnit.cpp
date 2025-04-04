#include "stdafx.h"
#include "PlayerUnit.h"

PlayerUnit::PlayerUnit(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path)
	: UnitBase(position, scale, rotation, path)
{
    // 텍스처 소스 생성
    auto srcPlayerIdleR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerIdleL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");
    
    auto srcPlayerRunR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerRunL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");
    
    auto srcPlayerJumpR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerJumpL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    auto srcPlayerJumpBetweenR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerJumpBetweenL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    auto srcPlayerJumpFallR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerJumpFallL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    auto srcPlayerAxeThrowPrepareR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerAxeThrowPrepareL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    auto srcPlayerAxeThrowR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerAxeThrowL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    auto srcPlayerAxeReturnR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerAxeReturnL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    auto srcPlayerCrouchR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerCrouchL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    auto srcPlayerCrouchWalkR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerCrouchWalkL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    auto srcPlayerHitR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerHitL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    auto srcPlayerDieR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerDieL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    // 애니메이션 클립 생성 (프레임 수, 시작 위치, 종료 위치, 역재생 여부 등은 이미지에 맞게 조정)
    auto playerIdleR = make_shared<AnimationClip>(L"IdleR", srcPlayerIdleR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerIdleR->GetWidth(), (float)srcPlayerIdleR->GetHeight()), false);
    auto playerIdleL = make_shared<AnimationClip>(L"IdleL", srcPlayerIdleL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerIdleL->GetWidth(), (float)srcPlayerIdleL->GetHeight()), true);

    auto playerRunR = make_shared<AnimationClip>(L"RunR", srcPlayerRunR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerRunR->GetWidth(), (float)srcPlayerRunR->GetHeight()), false);
    auto playerRunL = make_shared<AnimationClip>(L"RunL", srcPlayerRunL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerRunL->GetWidth(), (float)srcPlayerRunL->GetHeight()), true);

    auto playerJumpR = make_shared<AnimationClip>(L"JumpR", srcPlayerJumpR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpR->GetWidth(), (float)srcPlayerJumpR->GetHeight()), false);
    auto playerJumpL = make_shared<AnimationClip>(L"JumpL", srcPlayerJumpL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpL->GetWidth(), (float)srcPlayerJumpL->GetHeight()), true);

    auto playerJumpBetweenR = make_shared<AnimationClip>(L"JumpBetweenR", srcPlayerJumpBetweenR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpBetweenR->GetWidth(), (float)srcPlayerJumpBetweenR->GetHeight()), false);
    auto playerJumpBetweenL = make_shared<AnimationClip>(L"JumpBetweenL", srcPlayerJumpBetweenL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpBetweenL->GetWidth(), (float)srcPlayerJumpBetweenL->GetHeight()), true);

    auto playerJumpFallR = make_shared<AnimationClip>(L"JumpFallR", srcPlayerJumpFallR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpFallR->GetWidth(), (float)srcPlayerJumpFallR->GetHeight()), false);
    auto playerJumpFallL = make_shared<AnimationClip>(L"JumpFallL", srcPlayerJumpFallL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpFallL->GetWidth(), (float)srcPlayerJumpFallL->GetHeight()), true);

    auto playerAxeThrowPrepareR = make_shared<AnimationClip>(L"AxeThrowPrepareR", srcPlayerAxeThrowPrepareR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowPrepareR->GetWidth(), (float)srcPlayerAxeThrowPrepareR->GetHeight()), false);
    auto playerAxeThrowPrepareL = make_shared<AnimationClip>(L"AxeThrowPrepareL", srcPlayerAxeThrowPrepareL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowPrepareL->GetWidth(), (float)srcPlayerAxeThrowPrepareL->GetHeight()), true);

    auto playerAxeThrowR = make_shared<AnimationClip>(L"AxeThrowR", srcPlayerAxeThrowR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowR->GetWidth(), (float)srcPlayerAxeThrowR->GetHeight()), false);
    auto playerAxeThrowL = make_shared<AnimationClip>(L"AxeThrowL", srcPlayerAxeThrowL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowL->GetWidth(), (float)srcPlayerAxeThrowL->GetHeight()), true);

    auto playerAxeReturnR = make_shared<AnimationClip>(L"AxeReturnR", srcPlayerAxeReturnR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeReturnR->GetWidth(), (float)srcPlayerAxeReturnR->GetHeight()), false);
    auto playerAxeReturnL = make_shared<AnimationClip>(L"AxeReturnL", srcPlayerAxeReturnL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeReturnL->GetWidth(), (float)srcPlayerAxeReturnL->GetHeight()), true);

    auto playerCrouchR = make_shared<AnimationClip>(L"CrouchR", srcPlayerCrouchR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchR->GetWidth(), (float)srcPlayerCrouchR->GetHeight()), false);
    auto playerCrouchL = make_shared<AnimationClip>(L"CrouchL", srcPlayerCrouchL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchL->GetWidth(), (float)srcPlayerCrouchL->GetHeight()), true);

    auto playerCrouchWalkR = make_shared<AnimationClip>(L"CrouchWalkR", srcPlayerCrouchWalkR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchWalkR->GetWidth(), (float)srcPlayerCrouchWalkR->GetHeight()), false);
    auto playerCrouchWalkL = make_shared<AnimationClip>(L"CrouchWalkL", srcPlayerCrouchWalkL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchWalkL->GetWidth(), (float)srcPlayerCrouchWalkL->GetHeight()), true);

    auto playerHitR = make_shared<AnimationClip>(L"HitR", srcPlayerHitR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitR->GetWidth(), (float)srcPlayerHitR->GetHeight()), false);
    auto playerHitL = make_shared<AnimationClip>(L"HitL", srcPlayerHitL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitL->GetWidth(), (float)srcPlayerHitL->GetHeight()), true);

    auto playerDieR = make_shared<AnimationClip>(L"DieR", srcPlayerDieR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitR->GetWidth(), (float)srcPlayerHitR->GetHeight()), false);
    auto playerDieL = make_shared<AnimationClip>(L"DieL", srcPlayerDieL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitL->GetWidth(), (float)srcPlayerHitL->GetHeight()), true);


    // 애니메이터에 애니메이션 클립 추가
    GetAnimator()->AddAnimClip(playerIdleR);
    GetAnimator()->AddAnimClip(playerIdleL);
    GetAnimator()->AddAnimClip(playerRunR);
    GetAnimator()->AddAnimClip(playerRunL);
    GetAnimator()->AddAnimClip(playerJumpR);
    GetAnimator()->AddAnimClip(playerJumpL);
    GetAnimator()->AddAnimClip(playerJumpBetweenR);
    GetAnimator()->AddAnimClip(playerJumpBetweenL);
    GetAnimator()->AddAnimClip(playerJumpFallR);
    GetAnimator()->AddAnimClip(playerJumpFallL);
    GetAnimator()->AddAnimClip(playerAxeThrowPrepareR);
    GetAnimator()->AddAnimClip(playerAxeThrowPrepareL);
    GetAnimator()->AddAnimClip(playerAxeThrowR);
    GetAnimator()->AddAnimClip(playerAxeThrowL);
    GetAnimator()->AddAnimClip(playerAxeReturnR);
    GetAnimator()->AddAnimClip(playerAxeReturnL);
    GetAnimator()->AddAnimClip(playerCrouchR);
    GetAnimator()->AddAnimClip(playerCrouchL);
    GetAnimator()->AddAnimClip(playerCrouchWalkR);
    GetAnimator()->AddAnimClip(playerCrouchWalkL);
    GetAnimator()->AddAnimClip(playerHitR);
    GetAnimator()->AddAnimClip(playerHitL);
    GetAnimator()->AddAnimClip(playerDieR);
    GetAnimator()->AddAnimClip(playerDieL);

    GetAnimator()->SetAnimationTime(1.0f / 2.8f);

    this->AddComponent(make_shared<PlayerMove>());
    this->AddComponent(make_shared<AnimationMove>());
    this->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), scale, RED));
    if (this->GetComponent<BoxCollider>("BoxCollider")) COLLISION->AddCollider(this->GetComponent<BoxCollider>("BoxCollider"));
}

PlayerUnit::~PlayerUnit()
{
 
}

void PlayerUnit::Update()
{
	BASE::Update();
}

void PlayerUnit::Render()
{
	BASE::Render();
}
