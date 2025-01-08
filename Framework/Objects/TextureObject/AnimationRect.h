#pragma once

#include "TextureRect.h"

enum class AnimState
{
	ANIMATION_NONE = 0,
	ANIMATION_UP,
	ANIMATION_DOWN,
	ANIMATION_LEFT,
	ANIMATION_LEFT_UP,
	ANIMATION_LEFT_DOWN,
	ANIMATION_RIGHT,
	ANIMATION_RIGHT_UP,
	ANIMATION_RIGHT_DOWN
};

class AnimationRect : public TextureRect
{
private:
	shared_ptr<Animator> animator = nullptr;

	ComPtr<ID3D11BlendState> blend[2];
	ComPtr<ID3D11SamplerState> sampler[2];

	AnimState animState = AnimState::ANIMATION_LEFT;

public:
	AnimationRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path = L"");
	~AnimationRect();

public:
	void Update();
	void Render();

public:
	
};

