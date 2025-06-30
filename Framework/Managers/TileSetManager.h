#pragma once

class TileSetManager
{
    DECLARE_SINGLETON(TileSetManager);

public:
    void Init();

    // GUI 창에서 타일셋 팔레트를 표시
    void ImGUI();

    // SRV를 얻을 때는 ComPtr의 Get()을 사용
    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return tileSRV; }

    // 타일셋 변경: 새로운 이미지와 타일 분할 정보를 적용
    void ChangeTileMap(const std::wstring& imagePath, UINT xCount, UINT yCount);

public:
    wstring GetTilePath() { return tilePath; }

    UINT GetXCount() { return tileXCount; }
    UINT GetYCount() { return tileYCount; }

public:
    // 플레이어가 선택한 타일의 시작 UV와 이미지의 타일 크기
    Vector2 selectedStartUV = Values::ZeroVec2;
    Vector2 texelTileSize = Values::ZeroVec2;

private:
    // ComPtr으로 관리하여 자동으로 해제되도록 함
    ComPtr<ID3D11ShaderResourceView> tileSRV;

    UINT tileXCount = 0;
    UINT tileYCount = 0;

    wstring tilePath = L"";

};

