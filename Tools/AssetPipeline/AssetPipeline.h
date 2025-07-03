#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <wrl/client.h>

// 자산 변환기 인터페이스
class IAssetConverter
{
public:
    virtual ~IAssetConverter() = default;

    // srcPath 에 있는 파일을 dstPath 포맷으로 변환합니다.
    virtual void Convert(const std::wstring& srcPath, const std::wstring& dstPath) = 0;
};

// 여러 변환기를 관리하는 파이프라인 클래스
class AssetPipeline
{
public:
    // 변환기 등록
    void RegisterConverter(const std::string& name, std::shared_ptr<IAssetConverter> converter);

    // 등록된 변환기 실행
    void Convert(const std::string& name, const std::wstring& src, const std::wstring& dst);

private:
    std::unordered_map<std::string, std::shared_ptr<IAssetConverter>> converters;
};

// 텍스처 변환기 예시 구현
class TextureConverter : public IAssetConverter
{
public:
    void Convert(const std::wstring& srcPath, const std::wstring& dstPath) override;

private:
    // DirectXTex 사용을 위한 장치
    Microsoft::WRL::ComPtr<ID3D11Device> device;
};

