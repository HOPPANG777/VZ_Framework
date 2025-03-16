#include "stdafx.h"
#include "PlayerUnit.h"

PlayerUnit::PlayerUnit(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path)
	: UnitBase(position, scale, rotation, path)
{
    auto srcPlayerIdleR = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_R.png");
    auto srcPlayerIdleL = make_shared<Texture2D>(TexturePath + L"Animation/Player/Player_Idle_L.png");

    // AnimationClip 생성 (이름: "Loading")
    // - frameCount : 7
    // - startPos   : (0, 0)
    // - endPos     : (350, 50)  (tex->GetWidth(), tex->GetHeight() 라고 가정)
    auto playerIdleR = make_shared<AnimationClip>(L"IdleR", srcPlayerIdleR.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerIdleR->GetWidth(), (float)srcPlayerIdleR->GetHeight()), false);
    auto playerIdleL = make_shared<AnimationClip>(L"IdleL", srcPlayerIdleL.get(), 4, 1, Values::ZeroVec2, Vector2((float)srcPlayerIdleL->GetWidth(), (float)srcPlayerIdleL->GetHeight()), false);
    
    GetAnimator()->AddAnimClip(playerIdleR);
    GetAnimator()->AddAnimClip(playerIdleL);

    GetAnimator()->SetAnimationTime(1.0f / 2.8f);

    this->AddComponent(make_shared<PlayerMove>());
    this->AddComponent(make_shared<AnimationMove>());
    this->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(150, 160, 0), RED));
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
