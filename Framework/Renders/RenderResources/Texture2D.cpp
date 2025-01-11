#include "Framework.h"
#include "Texture2D.h"

using namespace DirectX;

// 정적 멤버 변수 초기화
vector<TextureDesc> Textures::descs;

// Texture2D 생성자
Texture2D::Texture2D(const wstring& filePath)
    : filePath(filePath)
{
    Textures::Load(this);
}

// Textures::Delete()는 descs에 들어있는
// 모든 srv를 명시적으로 해제한다.
void Textures::Delete()
{
    for (auto& desc : descs)
    {
        // ComPtr은 스코프를 벗어나면 자동 해제되지만,
        // 필요하다면 이렇게 Reset()으로 강제 해제 가능
        desc.srv.Reset();
    }
    descs.clear();
}

// 텍스처 로드 함수
void Textures::Load(Texture2D* texture)
{
    HRESULT hr = S_OK;
    TexMetadata metaData;

    // 확장자 추출
    wstring ext = Path::GetExtension(texture->filePath);

    // 확장자에 따라 메타데이터 얻기
    if (ext == L"tga")
    {
        hr = GetMetadataFromTGAFile(texture->filePath.c_str(), metaData);
        CHECK(hr);
    }
    else if (ext == L"dds")
    {
        hr = GetMetadataFromDDSFile(texture->filePath.c_str(),
            DDS_FLAGS_NONE, metaData);
        CHECK(hr);
    }
    else
    {
        hr = GetMetadataFromWICFile(texture->filePath.c_str(),
            WIC_FLAGS_NONE, metaData);
        CHECK(hr);
    }

    UINT width = metaData.width;
    UINT height = metaData.height;

    // TextureDesc 생성
    TextureDesc desc;
    desc.filePath = texture->filePath;
    desc.width = width;
    desc.height = height;

    TextureDesc exist;
    bool bExist = false;

    // 이미 같은 파일/크기의 TextureDesc가 있는지 확인
    for (auto& temp : descs)
    {
        if (desc == temp)
        {
            bExist = true;
            exist = temp;
            break;
        }
    }

    // 이미 존재한다면, 기존 SRV만 가져다가 사용
    if (bExist)
    {
        texture->srv = exist.srv;
        texture->metaData = metaData;
    }
    else
    {
        // 새로운 텍스처 로드
        ScratchImage image;
        if (ext == L"tga")
        {
            hr = LoadFromTGAFile(texture->filePath.c_str(), &metaData, image);
            CHECK(hr);
        }
        else if (ext == L"dds")
        {
            hr = LoadFromDDSFile(texture->filePath.c_str(),
                DDS_FLAGS_NONE, &metaData, image);
            CHECK(hr);
        }
        else
        {
            hr = LoadFromWICFile(texture->filePath.c_str(),
                WIC_FLAGS_NONE, &metaData, image);
            CHECK(hr);
        }

        ComPtr<ID3D11ShaderResourceView> shaderResourceView;
        hr = CreateShaderResourceView(DEVICE,
            image.GetImages(),
            image.GetImageCount(),
            metaData,
            &shaderResourceView);
        CHECK(hr);

        desc.srv = shaderResourceView;
        texture->srv = shaderResourceView;
        texture->metaData = metaData;

        descs.push_back(desc);
    }
}

// ReadPixel 멤버 함수
void Texture2D::ReadPixel(vector<Color>* pixels)
{
    // srv에서 실제 Texture2D COM 포인터 가져오기
    ComPtr<ID3D11Resource> resource;
    srv->GetResource(&resource);

    ComPtr<ID3D11Texture2D> texture2D;
    resource.As(&texture2D);

    ReadPixel(texture2D, pixels);
}

// ReadPixel 정적 함수
void Texture2D::ReadPixel(ComPtr<ID3D11Texture2D> texture,
    vector<Color>* pixels)
{
    // 원본 텍스처 정보
    D3D11_TEXTURE2D_DESC srvDesc;
    texture->GetDesc(&srvDesc);

    // 복사본 desc 설정
    D3D11_TEXTURE2D_DESC destDesc = {};
    destDesc.Usage = D3D11_USAGE_STAGING;
    destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    destDesc.Width = srvDesc.Width;
    destDesc.Height = srvDesc.Height;
    destDesc.MipLevels = 1;
    destDesc.ArraySize = 1;
    destDesc.Format = srvDesc.Format;
    destDesc.SampleDesc = srvDesc.SampleDesc;

    // CreateTexture2D
    ComPtr<ID3D11Texture2D> destTex;
    HRESULT hr = DEVICE->CreateTexture2D(&destDesc, nullptr, &destTex);
    CHECK(hr);

    // 원본 texture를 복사본 destTex에 로드
    hr = D3DX11LoadTextureFromTexture(DC, texture.Get(), nullptr, destTex.Get());
    CHECK(hr);

    // 픽셀 데이터 담을 메모리(Width * Height)
    const size_t pixelCount = static_cast<size_t>(destDesc.Width)
        * static_cast<size_t>(destDesc.Height);

    // unique_ptr로 배열 관리
    unique_ptr<UINT[]> colors = make_unique<UINT[]>(pixelCount);

    // Map을 통해 CPU로 읽어온다
    D3D11_MAPPED_SUBRESOURCE subRe;
    DC->Map(destTex.Get(), 0, D3D11_MAP_READ, 0, &subRe);
    {
        // 복사
        memcpy(colors.get(), subRe.pData, sizeof(UINT) * pixelCount);
    }
    DC->Unmap(destTex.Get(), 0);

    // OpenGL 호환용(BGR → RGB 순서 교체)
    {
        // 임시 버퍼들
        unique_ptr<UINT[]> alpha = make_unique<UINT[]>(pixelCount);
        unique_ptr<UINT[]> red = make_unique<UINT[]>(pixelCount);
        unique_ptr<UINT[]> green = make_unique<UINT[]>(pixelCount);
        unique_ptr<UINT[]> blue = make_unique<UINT[]>(pixelCount);

        for (size_t i = 0; i < pixelCount; i++)
        {
            alpha[i] = (colors[i] & 0xFF000000);
            red[i] = (colors[i] & 0x00FF0000);
            green[i] = (colors[i] & 0x0000FF00);
            blue[i] = (colors[i] & 0x000000FF);

            // R과 B 위치 교체
            red[i] = colors[i] >> 16;    // 16비트 쉬프트 → R 추출
            blue[i] = colors[i] << 16;    // <<16 → B를 상위 비트로

            colors[i] = alpha[i] | red[i] | green[i] | blue[i];
        }

        // 최종 픽셀을 결과 벡터에 저장
        pixels->assign(colors.get(), colors.get() + pixelCount);
    }
}

