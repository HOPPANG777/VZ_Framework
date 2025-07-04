#include "stdafx.h"
#include "Wind.h"

Wind::Wind(const Vector3& position, const Vector3& scale, const float rotation, const std::wstring path)
	: AnimationRect(position, scale, rotation, path)
{
	const std::shared_ptr<Texture2D>& srcWindIdle = std::make_shared<Texture2D>(TexturePath + L"GameObject/Wind.png");
	const std::shared_ptr<AnimationClip>& playerIdle = std::make_shared<AnimationClip>
		(
			L"Idle", srcWindIdle.get(), 
			4, 
			1, 
			Values::ZeroVec2, 
			Vector2((float)srcWindIdle->GetWidth(), (float)srcWindIdle->GetHeight()), 
			false
		);

	GetAnimator()->AddAnimClip(playerIdle);

	GetAnimator()->SetAnimationTime(1.0f / 2.8f);

	GetAnimator()->SetCurrentAnimClip(L"Idle");
}

Wind::~Wind()
{

}

void Wind::Update()
{
	BASE::Update();
}

void Wind::Render()
{
	BASE::Render();
}
