#include "Framework.h"
#include "Texture2DManager.h"

std::vector<TextureDesc> Texture2DManager::descs;

Texture2DManager::Texture2DManager() {}

Texture2DManager::~Texture2DManager() {}

void Texture2DManager::Delete()
{
    for (auto& desc : descs)
    {
        desc.srv.Reset();
    }
    descs.clear();
}

// 텍스처2D를 생성하거나 따로 만들때 해당 texture2D의 데이터를 이용해서
// 이미 해당 그림의 데이터가 있는지 확인하고 없다면 만드는 함수
void Texture2DManager::Load(Texture2D* texture)
{
    HRESULT hr;
    TexMetadata metaData;

    // 파일 확장자 추출
    std::wstring ext = Path::GetExtension(texture->filePath);

    // 확장자에 따라 메타데이터 읽기
    if (ext == L"tga")
    {
        hr = GetMetadataFromTGAFile(texture->filePath.c_str(), metaData);
        CHECK(hr);
    }
    else if (ext == L"dds")
    {
        hr = GetMetadataFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_NONE, metaData);
        CHECK(hr);
    }
    else
    {
        hr = GetMetadataFromWICFile(texture->filePath.c_str(), WIC_FLAGS_NONE, metaData);
        CHECK(hr);
    }
    
    // 해당 그림의 너비와 높이를 변수화
    size_t width = metaData.width;
    size_t height = metaData.height;

    // Texture2D가 가지고있던 파일 경로 이미지의
    // 주소, 너비, 높이를 desc에 담음
    TextureDesc desc;
    desc.filePath = texture->filePath;
    desc.width = width;
    desc.height = height;

    // 만약 이미 가지고있는 descs에 이번에 만들려 하는 Texture2D의
    // 주소, 너비, 높이가 같은 이미지 데이터가 이미 있는지 검사
    bool bExist = false;
    TextureDesc existDesc;
    for (const auto& temp : descs)
    {
        if (desc == temp)
        {
            // 주소, 너비, 높이가 같은 이미지가 있을경우
            bExist = true;
            existDesc = temp;   // 이미 있는 데이터를 exist에 넣기
            break;
        }
    }

    // 텍스처 데이터가 이미 있다면
    if (bExist)
    {
        // 기존에 로드된 텍스처가 있으면 캐싱된 SRV 사용
        texture->srv = existDesc.srv;
        texture->metaData = metaData;
    }
    else // 이전에 만든 텍스쳐 데이터가 없다면
    {
        // 이미지 파일로 부터 텍스처 데이터를 추출하는데 사용
        ScratchImage image;

        if (ext == L"tga")
        {
            hr = LoadFromTGAFile(texture->filePath.c_str(), &metaData, image);
            CHECK(hr);
        }
        else if (ext == L"dds")
        {
            hr = LoadFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_NONE, &metaData, image);
            CHECK(hr);
        }
        else
        {
            hr = LoadFromWICFile(texture->filePath.c_str(), WIC_FLAGS_NONE, &metaData, image);
            CHECK(hr);
        }

        ComPtr<ID3D11ShaderResourceView> shaderResourceView;
        hr = CreateShaderResourceView
        (
            Graphics::Get()->GetDevice(),
            image.GetImages(),
            image.GetImageCount(),
            metaData,
            &shaderResourceView
        );
        CHECK(hr);

        desc.srv = shaderResourceView;
        texture->srv = shaderResourceView;
        texture->metaData = metaData;
        descs.push_back(desc);
    }
}
