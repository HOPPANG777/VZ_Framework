#include "stdafx.h"
#include "AssetPipeline.h"

void AssetPipeline::RegisterConverter(const std::string& name, std::shared_ptr<IAssetConverter> converter)
{
    converters[name] = converter;
}

void AssetPipeline::Convert(const std::string& name, const std::wstring& src, const std::wstring& dst)
{
    auto it = converters.find(name);
    if (it != converters.end())
        it->second->Convert(src, dst);
}

void TextureConverter::Convert(const std::wstring& srcPath, const std::wstring& dstPath)
{
    // 실제 변환 로직은 DirectXTex 등을 활용하여 구현합니다.
    // 여기서는 예제로 파일 경로를 출력만 합니다.
    std::wstring message = L"Convert Texture: " + srcPath + L" -> " + dstPath;
    OutputDebugStringW(message.c_str());
}

