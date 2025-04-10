#include "stdafx.h"
#include "PlayerUnit.h"

PlayerUnit::PlayerUnit(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path)
	: UnitBase(position, scale, rotation, path)
{
    // 텍스처 소스 생성
    const std::shared_ptr<Texture2D>& srcPlayerIdleR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerIdleL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");
    
    const std::shared_ptr<Texture2D>& srcPlayerRunR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Run_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerRunL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Run_L.png");
    
    const std::shared_ptr<Texture2D>& srcPlayerJumpR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Jump_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerJumpL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Jump_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerJumpBetweenR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Between_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerJumpBetweenL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Between_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerJumpFallR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Fall_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerJumpFallL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Fall_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerAxeThrowPrepareR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerAxeThrowPrepareL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerAxeThrowR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerAxeThrowL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerAxeReturnR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerAxeReturnL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerCrouchR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerCrouchL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerCrouchWalkR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerCrouchWalkL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerHitR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerHitL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerDieR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerDieL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    // 애니메이션 클립 생성 (프레임 수, 시작 위치, 종료 위치, 역재생 여부 등은 이미지에 맞게 조정)
    const std::shared_ptr<AnimationClip>& playerIdleR = make_shared<AnimationClip>(L"IdleR", srcPlayerIdleR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerIdleR->GetWidth(), (float)srcPlayerIdleR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerIdleL = make_shared<AnimationClip>(L"IdleL", srcPlayerIdleL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerIdleL->GetWidth(), (float)srcPlayerIdleL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerRunR = make_shared<AnimationClip>(L"RunR", srcPlayerRunR.get(), 8, 1, Values::ZeroVec2, Vector2((float)srcPlayerRunR->GetWidth(), (float)srcPlayerRunR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerRunL = make_shared<AnimationClip>(L"RunL", srcPlayerRunL.get(), 8, 1, Values::ZeroVec2, Vector2((float)srcPlayerRunL->GetWidth(), (float)srcPlayerRunL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerJumpR = make_shared<AnimationClip>(L"JumpR", srcPlayerJumpR.get(), 5, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpR->GetWidth(), (float)srcPlayerJumpR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerJumpL = make_shared<AnimationClip>(L"JumpL", srcPlayerJumpL.get(), 5, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpL->GetWidth(), (float)srcPlayerJumpL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerJumpBetweenR = make_shared<AnimationClip>(L"JumpBetweenR", srcPlayerJumpBetweenR.get(), 3, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpBetweenR->GetWidth(), (float)srcPlayerJumpBetweenR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerJumpBetweenL = make_shared<AnimationClip>(L"JumpBetweenL", srcPlayerJumpBetweenL.get(), 3, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpBetweenL->GetWidth(), (float)srcPlayerJumpBetweenL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerJumpFallR = make_shared<AnimationClip>(L"JumpFallR", srcPlayerJumpFallR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpFallR->GetWidth(), (float)srcPlayerJumpFallR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerJumpFallL = make_shared<AnimationClip>(L"JumpFallL", srcPlayerJumpFallL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpFallL->GetWidth(), (float)srcPlayerJumpFallL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerAxeThrowPrepareR = make_shared<AnimationClip>(L"AxeThrowPrepareR", srcPlayerAxeThrowPrepareR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowPrepareR->GetWidth(), (float)srcPlayerAxeThrowPrepareR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerAxeThrowPrepareL = make_shared<AnimationClip>(L"AxeThrowPrepareL", srcPlayerAxeThrowPrepareL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowPrepareL->GetWidth(), (float)srcPlayerAxeThrowPrepareL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerAxeThrowR = make_shared<AnimationClip>(L"AxeThrowR", srcPlayerAxeThrowR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowR->GetWidth(), (float)srcPlayerAxeThrowR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerAxeThrowL = make_shared<AnimationClip>(L"AxeThrowL", srcPlayerAxeThrowL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowL->GetWidth(), (float)srcPlayerAxeThrowL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerAxeReturnR = make_shared<AnimationClip>(L"AxeReturnR", srcPlayerAxeReturnR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeReturnR->GetWidth(), (float)srcPlayerAxeReturnR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerAxeReturnL = make_shared<AnimationClip>(L"AxeReturnL", srcPlayerAxeReturnL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeReturnL->GetWidth(), (float)srcPlayerAxeReturnL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerCrouchR = make_shared<AnimationClip>(L"CrouchR", srcPlayerCrouchR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchR->GetWidth(), (float)srcPlayerCrouchR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerCrouchL = make_shared<AnimationClip>(L"CrouchL", srcPlayerCrouchL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchL->GetWidth(), (float)srcPlayerCrouchL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerCrouchWalkR = make_shared<AnimationClip>(L"CrouchWalkR", srcPlayerCrouchWalkR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchWalkR->GetWidth(), (float)srcPlayerCrouchWalkR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerCrouchWalkL = make_shared<AnimationClip>(L"CrouchWalkL", srcPlayerCrouchWalkL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchWalkL->GetWidth(), (float)srcPlayerCrouchWalkL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerHitR = make_shared<AnimationClip>(L"HitR", srcPlayerHitR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitR->GetWidth(), (float)srcPlayerHitR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerHitL = make_shared<AnimationClip>(L"HitL", srcPlayerHitL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitL->GetWidth(), (float)srcPlayerHitL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerDieR = make_shared<AnimationClip>(L"DieR", srcPlayerDieR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitR->GetWidth(), (float)srcPlayerHitR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerDieL = make_shared<AnimationClip>(L"DieL", srcPlayerDieL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitL->GetWidth(), (float)srcPlayerHitL->GetHeight()), true);

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

    GetAnimator()->SetCurrentAnimClip(L"IdleR");

    this->AddComponent(make_shared<PlayerController>());
	this->AddComponent(make_shared<InteractionComponent>());
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
