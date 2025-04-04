#include "stdafx.h"
#include "LumberAxe.h"

LumberAxe::LumberAxe(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path)
	: AnimationRect(position, scale, rotation, path)
{
	SetName("LumberAxe");

    auto srcAxeIdleR = make_shared<Texture2D>(TexturePath + L"Weapon/Axe_R.png");
    auto srcAxeIdleL = make_shared<Texture2D>(TexturePath + L"Weapon/Axe_L.png");

    auto axeIdleR = make_shared<AnimationClip>(L"AxeIdle_R", srcAxeIdleR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleR->GetWidth(), (float)srcAxeIdleR->GetHeight()), false);
    auto axeIdleL = make_shared<AnimationClip>(L"AxeIdle_L", srcAxeIdleL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleL->GetWidth(), (float)srcAxeIdleL->GetHeight()), true);

    auto axePrepareR = make_shared<AnimationClip>(L"AxePrepare_R", srcAxeIdleR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleR->GetWidth(), (float)srcAxeIdleR->GetHeight()), false);
    auto axePrepareL = make_shared<AnimationClip>(L"AxePrepare_L", srcAxeIdleL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleL->GetWidth(), (float)srcAxeIdleL->GetHeight()), true);
    
    auto axeThrownR = make_shared<AnimationClip>(L"AxeThrown_R", srcAxeIdleR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleR->GetWidth(), (float)srcAxeIdleR->GetHeight()), false);
    auto axeThrownL = make_shared<AnimationClip>(L"AxeThrown_L", srcAxeIdleL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleL->GetWidth(), (float)srcAxeIdleL->GetHeight()), true);
    
    auto axeReturningR = make_shared<AnimationClip>(L"AxeReturning_R", srcAxeIdleR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleR->GetWidth(), (float)srcAxeIdleR->GetHeight()), false);
    auto axeReturningL = make_shared<AnimationClip>(L"AxeReturning_L", srcAxeIdleL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcAxeIdleL->GetWidth(), (float)srcAxeIdleL->GetHeight()), true);

    GetAnimator()->AddAnimClip(axeIdleR);
    GetAnimator()->AddAnimClip(axeIdleL);
    GetAnimator()->AddAnimClip(axePrepareR);
    GetAnimator()->AddAnimClip(axePrepareL);
    GetAnimator()->AddAnimClip(axeThrownR);
    GetAnimator()->AddAnimClip(axeThrownL);
    GetAnimator()->AddAnimClip(axeReturningR);
    GetAnimator()->AddAnimClip(axeReturningL);

    GetAnimator()->SetAnimationTime(1.0f / 2.8f);

	lumberAxeComponent = make_shared<LumberAxeComponent>();
	AddComponent(lumberAxeComponent);
	AddComponent(make_shared<LumberAxeAnimation>());

	AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), scale));
    if (this->GetComponent<BoxCollider>("BoxCollider")) COLLISION->AddCollider(this->GetComponent<BoxCollider>("BoxCollider"));
}

LumberAxe::~LumberAxe()
{

}

void LumberAxe::Update()
{
	BASE::Update();

    // 'E' 키 입력과 타겟이 설정되어 있을 경우 회수 시작
    if (INPUT->KeyPressed('E'))
    {
		lumberAxeComponent->Recall();
    }
}

void LumberAxe::Render()
{
	BASE::Render();
}