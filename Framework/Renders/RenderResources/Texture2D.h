#pragma once

class Texture2DManager;

class Texture2D
{
public:
    Texture2D(const wstring& filePath);

public:
    void ReadPixel(vector<Color>* pixels);
    static void ReadPixel(ComPtr<ID3D11Texture2D> texture, vector<Color>* pixels);

public:
    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return srv; }

    wstring GetFilePath() const { return filePath; }
    
    float GetWidth() const { return static_cast<float>(metaData.width); }
    float GetHeight() const { return static_cast<float>(metaData.height); }

private:
    ComPtr<ID3D11ShaderResourceView> srv;
    DirectX::TexMetadata metaData;
    wstring filePath = L"";

private:
    friend class Texture2DManager;

};