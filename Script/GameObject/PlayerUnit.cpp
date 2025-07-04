#include "stdafx.h"
#include "PlayerUnit.h"

PlayerUnit::PlayerUnit(const Vector3& position, const Vector3& scale, const float rotation, const std::wstring& path)
	: UnitBase(position, scale, rotation, path)
{
    // 텍스처 소스 생성
    const std::shared_ptr<Texture2D>& srcPlayerIdleR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerIdleL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");
    
    const std::shared_ptr<Texture2D>& srcPlayerRunR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Run_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerRunL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Run_L.png");
    
    const std::shared_ptr<Texture2D>& srcPlayerJumpR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Jump_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerJumpL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Jump_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerJumpBetweenR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Between_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerJumpBetweenL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Between_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerJumpFallR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Fall_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerJumpFallL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Fall_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerAxeThrowPrepareR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Axe_Prepare_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerAxeThrowPrepareL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Axe_Prepare_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerAxeThrowPrepareHoldR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Axe_Prepare_Hold_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerAxeThrowPrepareHoldL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Axe_Prepare_Hold_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerAxeThrowR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Axe_Throw_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerAxeThrowL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Axe_Throw_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerAxeReturnR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Axe_Return_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerAxeReturnL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Axe_Return_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerCrouchR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Crouch_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerCrouchL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Crouch_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerCrouchWalkR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Crouch_Walk_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerCrouchWalkL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Crouch_Walk_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerCrouchHoldR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Crouch_Hold_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerCrouchHoldL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Crouch_Hold_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerHitR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Damaged_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerHitL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Damaged_L.png");

    const std::shared_ptr<Texture2D>& srcPlayerDieR = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Died_R.png");
    const std::shared_ptr<Texture2D>& srcPlayerDieL = std::make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Died_L.png");

    // 애니메이션 클립 생성 (프레임 수, 시작 위치, 종료 위치, 역재생 여부 등은 이미지에 맞게 조정)
    const std::shared_ptr<AnimationClip>& playerIdleR = std::make_shared<AnimationClip>(L"IdleR", srcPlayerIdleR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerIdleR->GetWidth(), (float)srcPlayerIdleR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerIdleL = std::make_shared<AnimationClip>(L"IdleL", srcPlayerIdleL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerIdleL->GetWidth(), (float)srcPlayerIdleL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerRunR = std::make_shared<AnimationClip>(L"RunR", srcPlayerRunR.get(), 8, 1, Values::ZeroVec2, Vector2((float)srcPlayerRunR->GetWidth(), (float)srcPlayerRunR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerRunL = std::make_shared<AnimationClip>(L"RunL", srcPlayerRunL.get(), 8, 1, Values::ZeroVec2, Vector2((float)srcPlayerRunL->GetWidth(), (float)srcPlayerRunL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerJumpR = std::make_shared<AnimationClip>(L"JumpR", srcPlayerJumpR.get(), 5, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpR->GetWidth(), (float)srcPlayerJumpR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerJumpL = std::make_shared<AnimationClip>(L"JumpL", srcPlayerJumpL.get(), 5, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpL->GetWidth(), (float)srcPlayerJumpL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerJumpBetweenR = std::make_shared<AnimationClip>(L"JumpBetweenR", srcPlayerJumpBetweenR.get(), 3, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpBetweenR->GetWidth(), (float)srcPlayerJumpBetweenR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerJumpBetweenL = std::make_shared<AnimationClip>(L"JumpBetweenL", srcPlayerJumpBetweenL.get(), 3, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpBetweenL->GetWidth(), (float)srcPlayerJumpBetweenL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerJumpFallR = std::make_shared<AnimationClip>(L"JumpFallR", srcPlayerJumpFallR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpFallR->GetWidth(), (float)srcPlayerJumpFallR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerJumpFallL = std::make_shared<AnimationClip>(L"JumpFallL", srcPlayerJumpFallL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerJumpFallL->GetWidth(), (float)srcPlayerJumpFallL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerAxePrepareR = std::make_shared<AnimationClip>(L"AxePrepareR", srcPlayerAxeThrowPrepareR.get(), 5, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowPrepareR->GetWidth(), (float)srcPlayerAxeThrowPrepareR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerAxePrepareL = std::make_shared<AnimationClip>(L"AxePrepareL", srcPlayerAxeThrowPrepareL.get(), 5, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowPrepareL->GetWidth(), (float)srcPlayerAxeThrowPrepareL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerAxePrepareHoldR = std::make_shared<AnimationClip>(L"AxePrepareHoldR", srcPlayerAxeThrowPrepareHoldR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowPrepareHoldR->GetWidth(), (float)srcPlayerAxeThrowPrepareR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerAxePrepareHoldL = std::make_shared<AnimationClip>(L"AxePrepareHoldL", srcPlayerAxeThrowPrepareHoldL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowPrepareHoldL->GetWidth(), (float)srcPlayerAxeThrowPrepareL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerAxeThrowR = std::make_shared<AnimationClip>(L"AxeThrowR", srcPlayerAxeThrowR.get(), 5, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowR->GetWidth(), (float)srcPlayerAxeThrowR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerAxeThrowL = std::make_shared<AnimationClip>(L"AxeThrowL", srcPlayerAxeThrowL.get(), 5, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeThrowL->GetWidth(), (float)srcPlayerAxeThrowL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerAxeReturnR = std::make_shared<AnimationClip>(L"AxeReturnR", srcPlayerAxeReturnR.get(), 7, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeReturnR->GetWidth(), (float)srcPlayerAxeReturnR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerAxeReturnL = std::make_shared<AnimationClip>(L"AxeReturnL", srcPlayerAxeReturnL.get(), 7, 1, Values::ZeroVec2, Vector2((float)srcPlayerAxeReturnL->GetWidth(), (float)srcPlayerAxeReturnL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerCrouchR = std::make_shared<AnimationClip>(L"CrouchR", srcPlayerCrouchR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchR->GetWidth(), (float)srcPlayerCrouchR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerCrouchL = std::make_shared<AnimationClip>(L"CrouchL", srcPlayerCrouchL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchL->GetWidth(), (float)srcPlayerCrouchL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerCrouchWalkR = std::make_shared<AnimationClip>(L"CrouchWalkR", srcPlayerCrouchWalkR.get(), 8, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchWalkR->GetWidth(), (float)srcPlayerCrouchWalkR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerCrouchWalkL = std::make_shared<AnimationClip>(L"CrouchWalkL", srcPlayerCrouchWalkL.get(), 8, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchWalkL->GetWidth(), (float)srcPlayerCrouchWalkL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerCrouchHoldR = std::make_shared<AnimationClip>(L"CrouchHoldR", srcPlayerCrouchHoldR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchHoldR->GetWidth(), (float)srcPlayerCrouchHoldR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerCrouchHoldL = std::make_shared<AnimationClip>(L"CrouchHoldL", srcPlayerCrouchHoldL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerCrouchHoldL->GetWidth(), (float)srcPlayerCrouchHoldL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerHitR = std::make_shared<AnimationClip>(L"HitR", srcPlayerHitR.get(), 6, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitR->GetWidth(), (float)srcPlayerHitR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerHitL = std::make_shared<AnimationClip>(L"HitL", srcPlayerHitL.get(), 6, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitL->GetWidth(), (float)srcPlayerHitL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& playerDieR = std::make_shared<AnimationClip>(L"DieR", srcPlayerDieR.get(), 27, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitR->GetWidth(), (float)srcPlayerHitR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& playerDieL = std::make_shared<AnimationClip>(L"DieL", srcPlayerDieL.get(), 27, 1, Values::ZeroVec2, Vector2((float)srcPlayerHitL->GetWidth(), (float)srcPlayerHitL->GetHeight()), true);

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
    GetAnimator()->AddAnimClip(playerAxePrepareR);
    GetAnimator()->AddAnimClip(playerAxePrepareL);
    GetAnimator()->AddAnimClip(playerAxePrepareHoldR);
    GetAnimator()->AddAnimClip(playerAxePrepareHoldL);
    GetAnimator()->AddAnimClip(playerAxeThrowR);
    GetAnimator()->AddAnimClip(playerAxeThrowL);
    GetAnimator()->AddAnimClip(playerAxeReturnR);
    GetAnimator()->AddAnimClip(playerAxeReturnL);
    GetAnimator()->AddAnimClip(playerCrouchR);
    GetAnimator()->AddAnimClip(playerCrouchL);
    GetAnimator()->AddAnimClip(playerCrouchWalkR);
    GetAnimator()->AddAnimClip(playerCrouchWalkL);
    GetAnimator()->AddAnimClip(playerCrouchHoldR);
    GetAnimator()->AddAnimClip(playerCrouchHoldL);
    GetAnimator()->AddAnimClip(playerHitR);
    GetAnimator()->AddAnimClip(playerHitL);
    GetAnimator()->AddAnimClip(playerDieR);
    GetAnimator()->AddAnimClip(playerDieL);

    GetAnimator()->SetAnimationTime(1.0f / 2.8f);

    GetAnimator()->SetCurrentAnimClip(L"IdleR");

    this->AddComponent(std::make_shared<PlayerController>());
	this->AddComponent(std::make_shared<InteractionComponent>());
    this->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), scale, RED));
    if (this->GetComponent<BoxCollider>("BoxCollider")) CollisionManager::Get()->AddCollider(this->GetComponent<BoxCollider>("BoxCollider"));
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
