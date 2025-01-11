#include "Framework.h"
#include "TextureRect.h"

//, texture(std::make_shared<Texture2D>(path))
TextureRect::TextureRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path)
	: Drawable("TextureRect", position, scale, rotation, ShaderPath + L"VertexTexture.hlsl")
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

}

TextureRect::~TextureRect()
{
}

void TextureRect::Update()
{
	BASE::Update();
}

void TextureRect::Render()
{
	BASE::Render();

	DC->PSSetSamplers(0, 1, &sampler[1]);
	
	//// SRV를 픽셀 셰이더에 바인딩
	DC->PSSetShaderResources(0, 1, srv.GetAddressOf());
	// 레스터라이저 및 블렌드 상태 설정
	DC->RSSetState(rs.Get());

	DC->OMSetBlendState(blend[1].Get(), nullptr, (UINT)0xFFFFFFFF);

	// 드로우 호출
	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
