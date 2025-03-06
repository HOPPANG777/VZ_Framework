#pragma once

class TileSetManager
{
    DECLARE_SINGLETON(TileSetManager);

public:
    void Init();
    void ImGUI();

    void ChangeTileMap(const wstring& imagePath, const UINT& xCount, const UINT& yCount);

public:
    wstring GetTilePath() const { return tilePath; }

    UINT GetXCount() const { return tileXCount; }
    UINT GetYCount() const { return tileYCount; }

    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return tileSRV; }

public:
    // 선택된 타일의 시작 UV와 타일 크기
    Vector2 selectedStartUV = Values::ZeroVec2;
    Vector2 texelTileSize = Values::ZeroVec2;

private:
    ComPtr<ID3D11ShaderResourceView> tileSRV = nullptr;
    UINT tileXCount = 0;
    UINT tileYCount = 0;
    wstring tilePath = L"";
};

