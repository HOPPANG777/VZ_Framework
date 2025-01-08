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
	
	//std::wcout << L"Creating Texture2D for file: " << filePath << std::endl;
	//try
	//{
	//	// 텍스처 로드
	//	Textures::Load(make_shared<Texture2D>(*this));
	//	std::wcout << L"Texture2D successfully created for file: " << filePath << std::endl;
	//}
	//catch (const std::exception& e)
	//{
	//	std::cerr << "Error during Texture2D creation: " << e.what() << std::endl;
	//}
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
	//std::wcout << L"Textures::Load called for texture: " << texture->filePath << std::endl;

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
	// 원본 텍스쳐 정보
	D3D11_TEXTURE2D_DESC srvDesc;
	texture->GetDesc(&srvDesc);

	// 원본 텍스쳐 복사본
	D3D11_TEXTURE2D_DESC destDesc = {};
	destDesc.Usage = D3D11_USAGE_STAGING;
	destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	destDesc.Width = srvDesc.Width;
	destDesc.Height = srvDesc.Height;
	destDesc.MipLevels = 1;
	destDesc.ArraySize = 1;
	destDesc.Format = srvDesc.Format;
	destDesc.SampleDesc = srvDesc.SampleDesc;

	ComPtr<ID3D11Texture2D> destTex;
	HRESULT hr = DEVICE->CreateTexture2D(&destDesc, nullptr, destTex.GetAddressOf());
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create texture.");
	}

	// 원본 texture를 복사본인 destTex에 집어 넣는다.
	hr = D3DX11LoadTextureFromTexture(DC, texture, nullptr, destTex.Get());
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to load texture.");
	}

	auto colors = std::make_unique<UINT[]>(destDesc.Width * destDesc.Height);

	D3D11_MAPPED_SUBRESOURCE subRe;
	DC->Map(destTex.Get(), 0, D3D11_MAP_READ, 0, &subRe);
	{
		// 그림의 픽셀 색상을 전부 추출
		memcpy(colors.get(), subRe.pData, sizeof(UINT) * destDesc.Width * destDesc.Height);
	}
	DC->Unmap(destTex.Get(), 0);

	// 이미지 가로 * 세로 즉, 픽셀 개수
	int pixelNum = destDesc.Width * destDesc.Height;

	// OpenGL과 호환되기 위해 색상 위치를 수정
	{
		auto alpha = std::make_unique<UINT[]>(pixelNum);
		auto red = std::make_unique<UINT[]>(pixelNum);
		auto green = std::make_unique<UINT[]>(pixelNum);
		auto blue = std::make_unique<UINT[]>(pixelNum);

		for (int i = 0; i < pixelNum; i++) {
			alpha[i] = (colors[i] & 0xFF000000);
			red[i] = (colors[i] & 0x00FF0000);
			green[i] = (colors[i] & 0x0000FF00);
			blue[i] = (colors[i] & 0x000000FF);

			// OpenGL은 ARGB가 아닌 ABGR 순서이기 때문에
			// R과 B의 위치를 바꾼다
			red[i] = colors[i] >> 16;
			blue[i] = colors[i] << 16;

			colors[i] = alpha[i] | red[i] | green[i] | blue[i];
		}

		// 결과를 벡터로 저장
		pixels.assign(&colors[0], &colors[pixelNum]);
	}
}

