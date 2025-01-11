#pragma once

struct TextureDesc
{
    wstring filePath = L"";
    UINT width = 0;
    UINT height = 0;

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

class Texture2D
{
public:
    Texture2D(const wstring& filePath);

    void ReadPixel(vector<Color>* pixels);
    static void ReadPixel(ComPtr<ID3D11Texture2D> texture,
        vector<Color>* pixels);

    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return srv; }
    float GetWidth() const { return static_cast<float>(metaData.width); }
    float GetHeight() const { return static_cast<float>(metaData.height); }
    wstring GetFilePath() const { return filePath; }

private:
    friend class Textures;

    wstring filePath = L"";
    ComPtr<ID3D11ShaderResourceView> srv;
    DirectX::TexMetadata metaData;
};

class Textures
{
public:
    static void Delete();

private:
    friend class Texture2D;

    static void Load(Texture2D* texture);
    static vector<TextureDesc> descs;
};