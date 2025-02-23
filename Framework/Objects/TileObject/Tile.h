#pragma once

class Tile : public Drawable
{
private:
    bool useTileSet;          // true: 타일셋 모드, false: 개별 타일 모드
    wstring texturePath;      // 타일셋 전체 이미지 경로나 개별 타일 텍스처 경로

    // 타일셋 모드용 정보 (개별 모드에서는 columns, rows는 1로 사용)
    UINT columns;
    UINT rows;
    UINT tileIndex;

    // 추가 상태 객체
    ComPtr<ID3D11SamplerState> sampler[2];
    ComPtr<ID3D11BlendState> blend[2];
    unique_ptr<ColorBuffer> colorBuffer;
    ComPtr<ID3D11RasterizerState> rs;

    // 정점 및 인덱스 데이터 (사각형: 4개의 정점, 6개의 인덱스)
    vector<VertexTile> vertices;
    vector<UINT> indices;

    // 텍스처 SRV (픽셀 셰이더에서 사용)
    ComPtr<ID3D11ShaderResourceView> srv;

private:
    // 내부: 텍스처 로드 및 UV 좌표 업데이트
    void LoadTexture();
    void UpdateUV();

    // 내부: 추가 렌더 상태 설정 (Sampler, Blend, ColorBuffer, Rasterizer 등)
    void SetupRenderStates();

public:
    Tile(const string& name, const Vector3& position, const Vector3& scale, float rotation, const wstring& texturePath); // 단일 타일 모드: 개별 타일 텍스처 사용 (UV는 (0,0)~(1,1) 고정)
    Tile(const string& name, const Vector3& position, const Vector3& scale, float rotation, const wstring& tileSetPath, UINT columns, UINT rows, UINT tileIndex); // 타일셋 모드: 하나의 타일셋 텍스처에서 지정한 타일 인덱스의 UV를 계산
    virtual ~Tile();

public:
    virtual void Update() override;
    virtual void Render() override;
    
public:
    void SetTileIndex(UINT index); // 타일셋 모드에서 타일 인덱스를 동적으로 변경

public:
    // (필요하다면) 현재 texturePath와 tileIndex를 가져오는 Getter 추가 가능
    wstring GetTexturePath() const { return texturePath; }
    UINT GetTileIndex() const { return tileIndex; }

};