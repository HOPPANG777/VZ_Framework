#include "stdafx.h"
#include "LumberAxe.h"

LumberAxe::LumberAxe(const Vector3& position, const Vector3& scale, const float rotation, const std::wstring& path)
	: AnimationRect(position, scale, rotation, path)
{
	SetName("LumberAxe");

    const std::shared_ptr<Texture2D>& srcAxeIdleR = std::make_shared<Texture2D>(TexturePath + L"Weapon/Axe/Axe_Idle_R.png");
    const std::shared_ptr<Texture2D>& srcAxeIdleL = std::make_shared<Texture2D>(TexturePath + L"Weapon/Axe/Axe_Idle_L.png");

    const std::shared_ptr<Texture2D>& srcAxeThrownR = std::make_shared<Texture2D>(TexturePath + L"Weapon/Axe/Axe_Thrown_R.png");
    const std::shared_ptr<Texture2D>& srcAxeThrownL = std::make_shared<Texture2D>(TexturePath + L"Weapon/Axe/Axe_Thrown_L.png");

    const std::shared_ptr<Texture2D>& srcAxeReturningR = std::make_shared<Texture2D>(TexturePath + L"Weapon/Axe/Axe_Return_R.png");
    const std::shared_ptr<Texture2D>& srcAxeReturningL = std::make_shared<Texture2D>(TexturePath + L"Weapon/Axe/Axe_Return_L.png");

    const std::shared_ptr<AnimationClip>& axeIdleR = std::make_shared<AnimationClip>(L"AxeIdle_R", srcAxeIdleR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleR->GetWidth(), (float)srcAxeIdleR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& axeIdleL = std::make_shared<AnimationClip>(L"AxeIdle_L", srcAxeIdleL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleL->GetWidth(), (float)srcAxeIdleL->GetHeight()), true);

    const std::shared_ptr<AnimationClip>& axePrepareR = std::make_shared<AnimationClip>(L"AxePrepare_R", srcAxeIdleR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleR->GetWidth(), (float)srcAxeIdleR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& axePrepareL = std::make_shared<AnimationClip>(L"AxePrepare_L", srcAxeIdleL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleL->GetWidth(), (float)srcAxeIdleL->GetHeight()), true);
    
    const std::shared_ptr<AnimationClip>& axeThrownR = std::make_shared<AnimationClip>(L"AxeThrown_R", srcAxeThrownR.get(), 9, 1, Values::ZeroVec2, Vector2((float)srcAxeThrownR->GetWidth(), (float)srcAxeThrownR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& axeThrownL = std::make_shared<AnimationClip>(L"AxeThrown_L", srcAxeThrownL.get(), 9, 1, Values::ZeroVec2, Vector2((float)srcAxeThrownL->GetWidth(), (float)srcAxeThrownL->GetHeight()), true);
    
    const std::shared_ptr<AnimationClip>& axeReturningR = std::make_shared<AnimationClip>(L"AxeReturning_R", srcAxeReturningR.get(), 8, 1, Values::ZeroVec2, Vector2((float)srcAxeReturningR->GetWidth(), (float)srcAxeReturningR->GetHeight()), false);
    const std::shared_ptr<AnimationClip>& axeReturningL = std::make_shared<AnimationClip>(L"AxeReturning_L", srcAxeReturningL.get(), 8, 1, Values::ZeroVec2, Vector2((float)srcAxeReturningL->GetWidth(), (float)srcAxeReturningL->GetHeight()), true);

    GetAnimator()->AddAnimClip(axeIdleR);
    GetAnimator()->AddAnimClip(axeIdleL);
    GetAnimator()->AddAnimClip(axePrepareR);
    GetAnimator()->AddAnimClip(axePrepareL);
    GetAnimator()->AddAnimClip(axeThrownR);
    GetAnimator()->AddAnimClip(axeThrownL);
    GetAnimator()->AddAnimClip(axeReturningR);
    GetAnimator()->AddAnimClip(axeReturningL);

    GetAnimator()->SetAnimationTime(1.0f / 2.8f);

	lumberAxeComponent = std::make_shared<LumberAxeComponent>();
	AddComponent(lumberAxeComponent);
	AddComponent(std::make_shared<LumberAxeAnimation>());

	AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), scale * 0.5f));
    if (this->GetComponent<BoxCollider>("BoxCollider")) CollisionManager::Get()->AddCollider(this->GetComponent<BoxCollider>("BoxCollider"));
}

LumberAxe::~LumberAxe()
{

}

void LumberAxe::Update()
{
	BASE::Update();

    // 'E' 키 입력과 타겟이 설정되어 있을 경우 회수 시작
    if (Input::Get()->KeyPressed('E'))
    {
		lumberAxeComponent->Recall();
    }
}

void LumberAxe::Render()
{
	BASE::Render();
}