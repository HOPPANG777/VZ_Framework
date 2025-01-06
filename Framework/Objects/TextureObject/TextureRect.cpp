#include "Framework.h"
#include "TextureRect.h"


TextureRect::TextureRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path)
	: Drawable("TextureRect", position, scale, rotation, ShaderPath + L"VertexTexture.hlsl")//, texture(std::make_shared<Texture2D>(path))
{


	// 디버깅 메시지
	//if (!texture)
	//{
	//	std::wcerr << L"Error: Failed to create Texture2D for path: " << path << std::endl;
	//}
	//else if (!texture->GetSRV())
	//{
	//	std::wcerr << L"Error: Texture2D created but SRV is null for path: " << path << std::endl;
	//}
	//else
	//{
	//	std::wcout << L"TextureRect successfully initialized with Texture2D: "
	//		<< path << std::endl;
	//}

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

		// FillMode : 체우는 방식 설정
		// d3d11_fill_solid == 전부 체우겠다. (빈공간 안만든다)
		desc.FillMode = D3D11_FILL_SOLID;

		// 시계방향 설정 (false == 시계 방향 // true == 반 시계 방향)
		desc.FrontCounterClockwise = false;

		HRESULT hr = DEVICE->CreateRasterizerState(&desc, &rs);
		CHECK(hr);
	}

	{
		// 기존 srv 자동 해제 (ComPtr이 자동으로 관리)
		srv.Reset();

		// DirectXTex를 이용하여 텍스처 로드
		DirectX::ScratchImage scratchImage;
		HRESULT hr = DirectX::LoadFromWICFile(
			path.c_str(),
			DirectX::WIC_FLAGS_IGNORE_SRGB,
			nullptr,
			scratchImage
		);
		CHECK(hr); // 로드 실패 시 확인

		// 텍스처 데이터를 GPU에 업로드
		const DirectX::TexMetadata& metadata = scratchImage.GetMetadata();
		ComPtr<ID3D11Texture2D> texture;
		hr = DirectX::CreateTexture(
			DEVICE,
			scratchImage.GetImages(),
			scratchImage.GetImageCount(),
			metadata,
			reinterpret_cast<ID3D11Resource**>(texture.GetAddressOf())
		);
		CHECK(hr); // 텍스처 생성 실패 시 확인

		// Shader Resource View 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = metadata.format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		hr = DEVICE->CreateShaderResourceView(texture.Get(), &srvDesc, srv.GetAddressOf());
		CHECK(hr); // SRV 생성 실패 시 확인
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
	//// SRV를 픽셀 셰이더에 바인딩
	//if (!texture || !texture->GetSRV())
	//{
	//	std::wcerr << L"Error: Texture or SRV is not set in Render for texture: "
	//		<< texture->GetFilePath() << std::endl;
	//	return;
	//}

	// 텍스처의 SRV를 설정
	//DC->PSSetShaderResources(0, 1, texture->GetSRV().GetAddressOf());
	DC->PSSetShaderResources(0, 1, srv.GetAddressOf());

	// 디버깅 메시지
	//std::wcout << L"SRV successfully bound to slot 0 for texture: "
	//	<< texture->GetFilePath() << std::endl;

	//if (!texture || !texture->GetSRV())
	//{
	//	std::wcerr << L"Error: Texture or SRV not set in Render for texture: "
	//		<< texture->GetFilePath() << std::endl;
	//	return;
	//}
	//std::wcout << L"Binding SRV to Pixel Shader for texture: "
	//	<< texture->GetFilePath() << std::endl;

	// 레스터라이저 및 블렌드 상태 설정
	DC->RSSetState(rs.Get());
	DC->OMSetBlendState(blend[1].Get(), nullptr, (UINT)0xFFFFFFFF);
	DC->PSSetSamplers(0, 1, &sampler[1]);

	BASE::Render();

	// 드로우 호출
	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
