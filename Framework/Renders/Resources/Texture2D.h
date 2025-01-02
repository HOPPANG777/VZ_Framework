#pragma once

struct TextureDesc
{
    wstring filePath = L"";
    UINT width = 0;
    UINT height = 0;
    ComPtr<ID3D11ShaderResourceView> srv = nullptr;

    bool operator==(const TextureDesc& other) const
    {
        return filePath == other.filePath &&
            width == other.width &&
            height == other.height;
    }
};

class Texture2D
{
public:
    explicit Texture2D(const wstring& filePath);

    void ReadPixel(vector<Color>& pixels) const;
    static void ReadPixel(ID3D11Texture2D* texture, vector<Color>& pixels);

    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return srv; }
    float GetWidth() const { return static_cast<float>(metaData.width); }
    float GetHeight() const { return static_cast<float>(metaData.height); }
    wstring GetFilePath() const { return filePath; }

private:
    friend class Textures;

    wstring filePath;
    ComPtr<ID3D11ShaderResourceView> srv;
    DirectX::TexMetadata metaData;
};

class Textures
{
public:
    static void Delete();

private:
    friend class Texture2D;

    static void Load(shared_ptr<Texture2D> texture);
    static vector<TextureDesc> descs;
};