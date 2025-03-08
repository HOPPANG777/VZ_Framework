#pragma once

class TileMap
{
private:
    vector<VertexTile> vertices;
    vector<UINT> indices;

    unique_ptr<VertexBuffer> vertexBuffer;
    unique_ptr<IndexBuffer> indexBuffer;

    unique_ptr<VertexShader> vertexShader;

    unique_ptr<InputLayout> inputLayout;
    
    unique_ptr<PixelShader> pixelShader;

    unique_ptr<WorldBuffer> worldBuffer;

    unique_ptr<IndexNumBuffer> indexNumBuffer;
    
    Matrix world = Values::Identity;

    UINT width = 0, height = 0, spacing = 0;

    D3D11_MAPPED_SUBRESOURCE subResource;

    // 2차원 타일 배열: rows x cols, row-major 순서
    vector<vector<shared_ptr<Tile>>> tiles;

public:
    TileMap(UINT width, UINT height, UINT spacing);
    ~TileMap() = default;

public:
    void Update();
    void Render();

    void ImGUI();

public:
    void GenerateTileMap();

    void MapVertexBuffer();
    void UnmapVertexBuffer();

    void UpdateVertexBuffer();

public:
    void Save(const string& path);
    void Load(const string& path);

public:
    // 타일 하나를 반환 (행,열 계산)
    Tile* GetTile(Vector3 mPos);

};