#pragma once

struct TextureDesc
{
    std::wstring filePath = L"";
    size_t width = 0;
    size_t height = 0;

    // COM 인터페이스는 ComPtr로 관리
    ComPtr<ID3D11ShaderResourceView> srv;

    bool operator==(const TextureDesc& other) const
    {
        bool b = true;
        b &= (filePath == other.filePath);
        b &= (width == other.width);
        b &= (height == other.height);

        // srv는 실제 비교에서 제외(원본 코드와 동일).
        // 필요하다면 srv.Get()를 비교하도록 추가 가능.
        return b;
    }
};