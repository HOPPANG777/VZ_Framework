#include "Framework.h"
#include "Texture2D.h"

using namespace DirectX;
vector<TextureDesc> Textures::descs;

Texture2D::Texture2D(const wstring& filePath)
	: filePath(filePath)
{
	// 텍스처를 로드합니다.
	// 텍스처가 이미 존재하는 경우 기존 데이터를 사용하고,
	// 없을 경우 새로 생성합니다.
	Textures::Load(make_shared<Texture2D>(*this));
}

void Textures::Delete()
{
	// 저장된 모든 텍스처 데이터를 해제합니다.
	for (auto& desc : descs)
	{
		// Shader Resource View(SRV)를 초기화하여 DirectX 리소스를 해제합니다.
		desc.srv.Reset();
	}
	// 텍스처 목록을 비웁니다.
	descs.clear();
}

// 텍스처2D를 생성하거나 따로 만들때 해당 texture2D의 데이터를 이용해서
// 이미 해당 그림의 데이터가 있는지 확인하고 없다면 만드는 함수
void Textures::Load(shared_ptr<Texture2D> texture)
{
	HRESULT hr;
	TexMetadata metaData;

	// 텍스처 파일 경로의 확장자를 가져옵니다.
	// 확장자에 따라 적절한 로딩 함수를 호출합니다.
	wstring ext = Path::GetExtension(texture->filePath);
	if (ext == L"tga")
	{
		// TGA 파일에서 메타데이터를 가져옵니다.
		hr = GetMetadataFromTGAFile(texture->filePath.c_str(), metaData);
	}
	else if (ext == L"dds")
	{
		// DDS 파일에서 메타데이터를 가져옵니다.
		hr = GetMetadataFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_NONE, metaData);
	}
	else
	{
		// 그 외 확장자는 WIC 파일에서 메타데이터를 가져옵니다.
		hr = GetMetadataFromWICFile(texture->filePath.c_str(), WIC_FLAGS_NONE, metaData);
	}
	CHECK(hr); // HRESULT를 확인하고 실패 시 프로그램을 종료합니다.

	// 텍스처의 너비와 높이를 메타데이터에서 가져옵니다.
	UINT width = metaData.width;
	UINT height = metaData.height;


	// 텍스처의 메타정보를 기반으로 TextureDesc 구조체를 생성합니다.
	TextureDesc desc = { texture->filePath, width, height, nullptr };

	// 텍스처가 이미 로드되었는지 확인합니다.
	for (const auto& existingDesc : descs)
	{
		if (desc == existingDesc)
		{
			// 기존 텍스처가 존재하면 해당 Shader Resource View(SRV)와 메타데이터를 사용합니다.
			texture->srv = existingDesc.srv;
			texture->metaData = metaData;
			return;
		}
	}

	// 텍스처 데이터를 로드합니다.
	ScratchImage image;
	if (ext == L"tga")
	{
		hr = LoadFromTGAFile(texture->filePath.c_str(), &metaData, image);
	}
	else if (ext == L"dds")
	{
		hr = LoadFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_NONE, &metaData, image);
	}
	else
	{
		hr = LoadFromWICFile(texture->filePath.c_str(), WIC_FLAGS_NONE, &metaData, image);
	}
	CHECK(hr); // 로드 실패 시 프로그램을 종료합니다.

	// Shader Resource View(SRV)를 생성합니다.
	ComPtr<ID3D11ShaderResourceView> srv;
	hr = CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), metaData, srv.ReleaseAndGetAddressOf());
	CHECK(hr);  // SRV 생성 실패 시 프로그램을 종료합니다.

	// 생성된 데이터를 TextureDesc와 Texture2D에 저장합니다.
	desc.srv = srv;
	texture->srv = srv;
	texture->metaData = metaData;

	// 텍스처를 목록에 추가합니다.
	descs.push_back(desc);
}

void Texture2D::ReadPixel(vector<Color>& pixels) const
{
	// Shader Resource View에서 원본 텍스처를 가져옵니다.
	ComPtr<ID3D11Texture2D> texture;
	srv->GetResource(reinterpret_cast<ID3D11Resource**>(texture.GetAddressOf()));

	// 가져온 텍스처 데이터를 기반으로 픽셀 데이터를 읽어옵니다.
	ReadPixel(texture.Get(), pixels);
}

void Texture2D::ReadPixel(ID3D11Texture2D* texture, vector<Color>& pixels)
{
	// 원본 텍스처의 설명(D3D11_TEXTURE2D_DESC)을 가져옵니다.
	D3D11_TEXTURE2D_DESC srvDesc;
	texture->GetDesc(&srvDesc);

	// 읽기 전용 텍스처(복사본)의 설명을 설정합니다.
	D3D11_TEXTURE2D_DESC destDesc = {};
	destDesc.Usage = D3D11_USAGE_STAGING;  // CPU에서 읽을 수 있는 스테이징 텍스처로 설정
	destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;  // 읽기 전용
	destDesc.Width = srvDesc.Width;
	destDesc.Height = srvDesc.Height;
	destDesc.MipLevels = 1;  // MipMap 레벨 1로 설정
	destDesc.ArraySize = 1;  // 배열 크기 1로 설정
	destDesc.Format = srvDesc.Format;  // 포맷 동일
	destDesc.SampleDesc = srvDesc.SampleDesc;  // 샘플링 설명 동일

	// 읽기 전용 텍스처를 생성합니다.
	ComPtr<ID3D11Texture2D> destTex;
	HRESULT hr = DEVICE->CreateTexture2D(&destDesc, nullptr, destTex.GetAddressOf());
	CHECK(hr);  // 생성 실패 시 프로그램 종료

	// 원본 텍스처 데이터를 읽기 전용 텍스처에 복사합니다.
	DC->CopyResource(destTex.Get(), texture);

	// 복사된 텍스처 데이터를 매핑하여 접근할 수 있도록 설정합니다.
	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(destTex.Get(), 0, D3D11_MAP_READ, 0, &subResource);

	// 픽셀 데이터를 저장할 벡터 크기를 설정하고 데이터를 복사합니다.
	pixels.resize(destDesc.Width * destDesc.Height);
	memcpy(pixels.data(), subResource.pData, sizeof(Color) * pixels.size());

	// 매핑을 해제합니다.
	DC->Unmap(destTex.Get(), 0);
}

