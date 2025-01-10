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

class AnimationRect : public Drawable
{
private:
	shared_ptr<Animator> animator = nullptr;

	AnimState animState = AnimState::ANIMATION_LEFT;

	vector<VertexTexture> vertices;

	vector<UINT> indices;

	ComPtr<ID3D11RasterizerState> rs = nullptr;

	ComPtr<ID3D11BlendState> blend[2];

	ComPtr<ID3D11SamplerState> sampler[2];

	ComPtr<ID3D11ShaderResourceView> srv = nullptr;

	D3D11_MAPPED_SUBRESOURCE subRe;

public:
	AnimationRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path = L"");
	~AnimationRect();

public:
	void Update();
	void Render();

public:
	void SetSRV(ID3D11ShaderResourceView* srv) { this->srv = srv; }
	void SetImage(wstring imagePath)
	{
		// SRV 추출 // 그림 데이터 추출
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			DEVICE,
			imagePath.c_str(),
			nullptr,
			nullptr,
			&srv,
			nullptr
		);
		CHECK(hr);
	}

};

