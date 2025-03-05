#pragma once

class Tile : public Drawable, public IColorable
{
private:
    bool useTileSet;          // true: 타일셋 모드, false: 개별 타일 모드
    wstring texturePath;      // 개별 타일 텍스처 경로나 타일셋 이미지 경로

    // 타일셋 모드용 정보 (개별 모드에서는 columns, rows는 1로 사용)
    UINT columns;
    UINT rows;
    UINT tileIndex;

    // UV 좌표 (타일셋 모드: 계산된 UV, 일반 모드: (0,0)-(1,1))
    Vector2 startUV;
    Vector2 endUV;

    // 타일 색상
    Color color;

    // 추가 렌더 상태들
    ComPtr<ID3D11SamplerState> sampler[2];
    ComPtr<ID3D11BlendState> blend[2];
    ComPtr<ID3D11RasterizerState> rs = nullptr;
    unique_ptr<ColorBuffer> colorBuffer;

    // 정점 및 인덱스 데이터
    vector<VertexTile> vertices;
    vector<UINT> indices;

    // 텍스처 SRV (픽셀 셰이더에서 사용)
    ComPtr<ID3D11ShaderResourceView> srv = nullptr;

public:
    // 생성자: 일반 타일 모드
    Tile(const string& name, const Vector3& position, const Vector3& scale, float rotation, const wstring& texturePath);
    // 생성자: 타일셋 모드
    Tile(const string& name, const Vector3& position, const Vector3& scale, float rotation, const wstring& tileSetPath, UINT columns, UINT rows, UINT tileIndex);
    virtual ~Tile();

public:
    virtual void Update() override;
    virtual void Render() override;

public:
    // 텍스처 로드 및 UV 갱신
    void LoadTexture();
    void UpdateUV();
    void SetupRenderStates();

public:
    // 타일셋 모드에서 타일 인덱스 변경
    void SetTileIndex(UINT index);

public:
    // Getter
    wstring GetTexturePath() const { return texturePath; }
    UINT GetTileIndex() const { return tileIndex; }

public:
    // IColorable 구현 (만약 IColorable을 상속받도록 확장한다면)
    virtual void SetColor(const Color& col) override { color = col; UpdateUV(); }
    virtual Color GetColor() const override { return color; }

};