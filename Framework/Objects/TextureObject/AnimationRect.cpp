#include "Framework.h"
#include "AnimationRect.h"

AnimationRect::AnimationRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path)
	: Drawable("AnimationRect", position, scale, rotation, ShaderPath + L"VertexTexture.hlsl"), subRe(subRe)
{
	//vertices
	{
		vertices.assign(4, VertexTexture());

		// 좌 하단
		vertices[0].position = { -0.5f, -0.5f, 0 };
		vertices[0].uv = Vector2(0, 1);

		// 좌 상단
		vertices[1].position = { -0.5f, 0.5f, 0 };
		vertices[1].uv = Vector2(0, 0);

		// 우 하단
		vertices[2].position = { 0.5f, -0.5f, 0 };
		vertices[2].uv = Vector2(1, 1);

		// 우 상단
		vertices[3].position = { 0.5f, 0.5f, 0 };
		vertices[3].uv = Vector2(1, 0);
	}

	//vertexBuffer
	{
		//객체를 만들어 주어야 한다.
		vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
	}

	//indices
	{
		indices = { 0, 1, 2, 2, 1, 3 };
	}

	//indexBuffer
	{
		indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
	}

	//inputLayout
	{
		inputLayout->Create(VertexTexture::ieDescs.data(), VertexTexture::ieDescs.size(), vertexShader->GetBlob());
	}

	// Rasterizer State
	// 레스터라이저는 버텍스 쉐이더로 찍은 정점들의 사이와 내부를
	// 채워주는 역할이다.
	// 이거 안만들면 기본값으로 넣어주고 만들면 만든거 넣어줌
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

		// CullMode : 그리지 않을 부분 설정
		// d3d11_cull_back == 뒷면 지우겠다. (반시계 방향 안그림)
		desc.CullMode = D3D11_CULL_BACK;
		//desc.CullMode = D3D11_CULL_NONE;

		// FillMode : 체우는 방식 설정
		// d3d11_fill_solid == 전부 체우겠다. (빈공간 안만든다)
		desc.FillMode = D3D11_FILL_SOLID;

		// 시계방향 설정 (false == 시계 방향 // true == 반 시계 방향)
		desc.FrontCounterClockwise = false;

		HRESULT hr = DEVICE->CreateRasterizerState(&desc, &rs);
		CHECK(hr);
	}

	// SRV 추출 // 그림 데이터 추출
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			DEVICE,
			path.c_str(),
			nullptr,
			nullptr,
			srv.GetAddressOf(),
			nullptr
		);
		CHECK(hr);
	}

    // Sampler
    {
        D3D11_SAMPLER_DESC desc;
        RenderStates::GetSamplerDesc(&desc);
        RenderStates::CreateSampler(&desc, &sampler[0]);

        // 선형보간
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        RenderStates::CreateSampler(&desc, &sampler[1]);
    }

    // Blend
    {
        D3D11_BLEND_DESC desc;
        RenderStates::GetBlendDesc(&desc);
        RenderStates::CreateBlend(&desc, &blend[0]);

        desc.RenderTarget[0].BlendEnable = true;
        RenderStates::CreateBlend(&desc, &blend[1]);
	}

	animator = make_shared<Animator>();

	{
		// Texture2D를 unique_ptr로 관리
		shared_ptr<Texture2D> srcTex = make_shared<Texture2D>(path);

		// AnimationClip은 shared_ptr로 관리
		shared_ptr<AnimationClip> RunD = make_shared<AnimationClip>(
			L"RunD",                              // 애니메이션 이름
			srcTex,								  // 애니메이션에 사용할 Texture2D
			4,                                    // 4칸 사용
			Values::ZeroVec2,                     // 초기 위치
			Vector2(srcTex->GetWidth() * 0.5f,    // 마지막 위치(자를 위치)
				srcTex->GetHeight() * 0.125f));

		shared_ptr<AnimationClip> RunL = make_shared<AnimationClip>(
			L"RunL",                              // 애니메이션 이름
			srcTex,								  // 애니메이션에 사용할 Texture2D
			4,                                    // 4칸 사용
			Vector2(0, srcTex->GetHeight() * 0.125f * 2),
			Vector2(srcTex->GetWidth() * 0.5f, srcTex->GetHeight() * 0.125f * 3)); // 마지막 위치(자를 위치)

		// Animator에 AnimationClip 추가
		animator->AddAnimClip(RunD);
		animator->AddAnimClip(RunL);

		// srcTex는 고유 소유권을 가진 unique_ptr이므로 명시적으로 삭제할 필요가 없음
	}

	animator->SetCurrentAnimClip(L"RunL");
}

AnimationRect::~AnimationRect() {}

void AnimationRect::Update()
{
    if (Keyboard::Get()->Press('A'))
    {
        animator->SetCurrentAnimClip(L"RunL");
    }

    else if (Keyboard::Get()->Press('S'))
    {
        animator->SetCurrentAnimClip(L"RunD");
    }

    animator->Update();

    DC->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subRe);
    {
        // 왼쪽 아래 0 , 1
        vertices[0].uv.x = animator->GetCurrentFrame().x;
        vertices[0].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

        // 오른쪽 위 1 , 0
        vertices[1].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
        vertices[1].uv.y = animator->GetCurrentFrame().y;

        // 오른쪽 아래 1 , 1
        vertices[2].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
        vertices[2].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

        // 좌 상단  0 , 0
        vertices[3].uv.x = animator->GetCurrentFrame().x;
        vertices[3].uv.y = animator->GetCurrentFrame().y;

        memcpy(subRe.pData, vertices.data(), sizeof(vertices[0]) * vertices.size()); 
    }
    DC->Unmap(vertexBuffer->GetResource(), 0);

    BASE::Update();
}

void AnimationRect::Render()
{
    BASE::Render();

    srv = animator->GetCurrentSRV();

	DC->PSSetSamplers(0, 1, &sampler[1]);
	
	//// SRV를 픽셀 셰이더에 바인딩
	DC->PSSetShaderResources(0, 1, srv.GetAddressOf());
	// 레스터라이저 및 블렌드 상태 설정
	DC->RSSetState(rs.Get());

	DC->OMSetBlendState(blend[1].Get(), nullptr, (UINT)0xFFFFFFFF);

	// 드로우 호출
	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
