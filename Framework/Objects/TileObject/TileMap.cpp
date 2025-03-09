#include "Framework.h"
#include "TileMap.h"

TileMap::TileMap(UINT width, UINT height, UINT spacing)
    : width(width), height(height), spacing(spacing)
{
    // 타일셋(이미지) 관련 설정은 TileSetManager 싱글톤을 사용
    // 타일셋 매니저는 내부에서 텍스처(SRV)와 타일 분할 정보를 관리합니다.
    TILESET->Init();

    // 타일맵 생성 (2차원 vector 형태)
    GenerateTileMap();

    //vertices
    {
        // 타일 하나를 그리기 위한 기본 버텍스 데이터 (사각형)
        vertices.resize(4);
        vertices[0].position = Values::ZeroVec3;
        vertices[1].position = Vector3(0.0f, (float)spacing, 0.0f);
        vertices[2].position = Vector3((float)spacing, 0.0f, 0.0f);
        vertices[3].position = Vector3((float)spacing, (float)spacing, 0.0f);
    }

    //indices
    {
        // 인덱스 데이터 (사각형 2개의 삼각형)
        indices = { 0, 1, 2, 2, 1, 3 };
    }

    //vertexBuffer
    {
        // D3D 리소스 생성 (Framework의 매크로나 unique_ptr 활용)
        vertexBuffer = make_unique<VertexBuffer>();
        vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
    }
    
    //indexBuffer
    {
        indexBuffer = make_unique<IndexBuffer>();
        indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
    }
    
    //vertexBuffer
    {
        vertexShader = make_unique<VertexShader>();
        vertexShader->Create(ShaderPath + L"VertexTile.hlsl", "VS");
    }

    //pixelShader
    {
        pixelShader = make_unique<PixelShader>();
        pixelShader->Create(ShaderPath + L"VertexTile.hlsl", "PS");
    }

    //inputLayout
    {
        inputLayout = make_unique<InputLayout>();
        inputLayout->Create(VertexTile::isDescs.data(), VertexTile::isDescs.size(), vertexShader->GetBlob());
    }

    //worldBuffer
    {
        worldBuffer = make_unique<WorldBuffer>();
        worldBuffer->SetWorld(world);
    }

    //indexNumBuffer
    {
        indexNumBuffer = make_unique<IndexNumBuffer>();
        indexNumBuffer->SetIndex(0);
    }

}

TileMap::~TileMap()
{
    tiles.clear();
}

void TileMap::GenerateTileMap()
{
    if (width == 0 || height == 0 || spacing == 0) assert(false);

    // 2차원 벡터를 사용하여 각 행에 대해 vector<shared_ptr<Tile>>를 생성합니다.
    //vector<vector<shared_ptr<Tile>>> tiles;
    tiles.resize(height);

    int index = 0;
    for (UINT y = 0; y < height; y++)
    {
        // 각 행의 열 크기를 미리 할당합니다.
        tiles[y].resize(width);
        for (UINT x = 0; x < width; x++)
        {
            // 각 타일을 make_shared로 생성합니다.
            tiles[y][x] = make_shared<Tile>();
            tiles[y][x]->SetPosition(Vector3(static_cast<float>(x * spacing), static_cast<float>(y * spacing), 0.0f));
            tiles[y][x]->SetIndex(index++);
        }
    }
}

void TileMap::MapVertexBuffer()
{
    DC->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
}

void TileMap::UnmapVertexBuffer()
{
    memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
    DC->Unmap(vertexBuffer->GetResource(), 0);
}

void TileMap::UpdateVertexBuffer()
{
    // 각 타일마다 버텍스 버퍼를 업데이트하여 그리기
    for (UINT y = 0; y < height; y++)
    {
        for (UINT x = 0; x < width; x++)
        {
            shared_ptr<Tile> tile = tiles[y][x];
            //if (!tile) continue;

            MapVertexBuffer();
            {
                // 각 타일에 맞게 버텍스 데이터 업데이트
                for (VertexTile& v : vertices)
                {
                    v.color = tile->GetColor();
                    v.index = tile->GetIndex();
                }

                vertices[0].uv = Vector2(tile->GetStartUV().x, tile->GetEndUV().y);
                vertices[1].uv = tile->GetStartUV();
                vertices[2].uv = tile->GetEndUV();
                vertices[3].uv = Vector2(tile->GetEndUV().x, tile->GetStartUV().y);

                // uv2 (추가 텍스처 좌표, 기본값)
                vertices[0].uv2 = Vector2(0, 1);
                vertices[1].uv2 = Vector2(0, 0);
                vertices[2].uv2 = Vector2(1, 1);
                vertices[3].uv2 = Vector2(1, 0);
            }
            UnmapVertexBuffer();

            world = Matrix::CreateTranslation(tile->GetPosition());
            worldBuffer->SetWorld(world);
            worldBuffer->SetVSBuffer(0);
            indexNumBuffer->SetPSBuffer(0);
            DC->DrawIndexed(indexBuffer->GetCount(), 0, 0);
        }
    }
}

void TileMap::Update()
{
    // 예시: 카메라 위치를 고려하여 마우스 좌표를 월드 좌표로 변환
    Vector3 mPos = MOUSE->GetMousePos();

    // 카메라가 직교 카메라이고, GetPosition()이 카메라의 월드 위치를 반환한다고 가정하면:
    mPos += CAMERA->GetPosition();

    Tile* tile = GetTile(mPos);
    if (tile) indexNumBuffer->SetIndex(tile->GetIndex());

    if (MOUSE->Press(0x0000))
    {
        if (tile)
        {
            tile->SetColor(Values::Red);
            Vector2 startUV = TILESET->selectedStartUV;
            Vector2 endUV = startUV + TILESET->texelTileSize;
            tile->SetStartUV(startUV);
            tile->SetEndUV(endUV);
        }
    }
}

void TileMap::Render()
{
    vertexBuffer->SetIA();
    indexBuffer->SetIA();
    inputLayout->SetIA();
    DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    vertexShader->SetShader();
    pixelShader->SetShader();

    // 타일셋 매니저에서 관리하는 SRV를 픽셀 셰이더에 바인딩
    DC->PSSetShaderResources(0, 1, TILESET->GetSRV().GetAddressOf());

    UpdateVertexBuffer();
}

void TileMap::ImGUI()
{
    // 타일셋 매니저의 GUI (타일 팔레트)
    TILESET->ImGUI();
}

void TileMap::Save(const string& path)
{
    ofstream fout(path);
    fout << to_string(width) << ' ' << to_string(height) << ' ' << to_string(spacing) << endl;

    // 타일셋 정보
    string imagePath = "";
    wstring tsPath = TILESET->GetTilePath();
    imagePath.assign(tsPath.begin(), tsPath.end());
    string xCount = to_string(TILESET->GetXCount());
    string yCount = to_string(TILESET->GetYCount());
    fout << imagePath << ' ' << xCount << ' ' << yCount << endl;

    // 각 타일의 UV 정보를 row-major 순서로 저장
    for (UINT y = 0; y < height; y++)
    {
        for (UINT x = 0; x < width; x++)
        {
            auto tile = tiles[y][x];
            fout    << to_string(tile->GetStartUV().x)  << ' '
                    << to_string(tile->GetStartUV().y)  << ' '
                    << to_string(tile->GetEndUV().x)    << ' '
                    << to_string(tile->GetEndUV().y)    << endl;
        }
    }
}

void TileMap::Load(const string& path)
{
    ifstream fin(path);
    if (fin.fail())
    {
        HelpMe::Get()->Log("파일이 존재하지 않습니다. 이름을 정확히 적어주세요.");
        return;
    }

    tiles.clear();

    char temp[256];

    // (1) width, height, spacing 읽기
    {
        fin.getline(temp, 256);
        istringstream iss(temp);
        iss >> width >> height >> spacing;
        if (width == 0 || height == 0 || spacing == 0)
            assert(false);
    }

    // (2) 타일셋 정보 읽기
    {
        fin.getline(temp, 256);
        istringstream iss(temp);
        string imagePath;
        UINT xCount = 0, yCount = 0;
        iss >> imagePath >> xCount >> yCount;
        wstring tsPath(imagePath.begin(), imagePath.end());
        TILESET->ChangeTileMap(tsPath, xCount, yCount);
    }

    // (3) 2차원 벡터 재생성
    tiles.resize(height);
    for (UINT y = 0; y < height; y++)
    {
        tiles[y].resize(width);
        for (UINT x = 0; x < width; x++)
        {
            // 타일 생성
            auto tile = make_shared<Tile>();

            // 위치 설정 (중요!)
            Vector3 position((float)(x * spacing), (float)(y * spacing), 0.0f);
            tile->SetPosition(position);

            // UV 정보 읽기
            fin.getline(temp, 256);
            istringstream uvStream(temp);
            float sx, sy, ex, ey;
            uvStream >> sx >> sy >> ex >> ey;
            tile->SetStartUV(Vector2(sx, sy));
            tile->SetEndUV(Vector2(ex, ey));

            // 타일을 tiles에 저장
            tiles[y][x] = tile;
        }
    }

    // 디버그용 백업
    Save(TilePath + "After.txt");
    HelpMe::Get()->Log("타일맵 불러오기 완료!");
}

Tile* TileMap::GetTile(Vector3 mPos)
{
    UINT x = (int)(mPos.x) / spacing;
    UINT y = (int)(mPos.y) / spacing;
    if (x < 0 || x >= width || y < 0 || y >= height) return nullptr;
    return tiles[y][x].get();
}

