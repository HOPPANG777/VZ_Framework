#pragma once
#include "Systems/Graphics.h"
#include "Interfaces/ITexture.h"
#include <vector>

class Texture2DManager;

// DirectX11 전용 텍스처 구현체입니다.
// 다른 그래픽 API 구현체를 추가하려면 ITexture 인터페이스를 상속받아 구현합니다.
class Texture2D : public ITexture
{
public:
    // 파일을 통해 텍스처를 생성합니다.
    Texture2D(const std::wstring& filePath);

public:
    // 텍스처의 픽셀 정보를 읽어옵니다.
    void ReadPixel(std::vector<Color>* pixels);
    // 정적 함수 : 특정 텍스처의 픽셀 정보를 읽습니다.
    static void ReadPixel(ComPtr<ID3D11Texture2D> texture, std::vector<Color>* pixels);

public:
    // 셰이더 리소스 뷰 반환
    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return srv; }

    // ITexture 구현 : 파일 경로 반환
    std::wstring GetFilePath() const override { return filePath; }
    
    // 텍스처 가로 길이
    float GetWidth() const { return static_cast<float>(metaData.width); }
    // 텍스처 세로 길이
    float GetHeight() const { return static_cast<float>(metaData.height); }

private:
    // DX11 Shader Resource View
    ComPtr<ID3D11ShaderResourceView> srv;
    // 로드한 텍스처의 메타 데이터
    DirectX::TexMetadata metaData;
    // 로드된 파일 경로
    std::wstring filePath = L"";

private:
    friend class Texture2DManager;

};