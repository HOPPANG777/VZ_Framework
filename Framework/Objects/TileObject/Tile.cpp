#include "Framework.h"
#include "Tile.h"

Tile::Tile(const string& name, const Vector3& position, const Vector3& scale, float rotation, const wstring& texturePath)
    : Drawable(name, position, scale, rotation, TransformType::Normal, ShaderPath + L"VertexTile.hlsl"), useTileSet(false), texturePath(texturePath), columns(1), rows(1), tileIndex(0), color(WHITE)
{
    // vertices
    {
        // 정사각형 타일: 4개 정점 생성
        vertices.resize(4);

        vertices[0] = VertexTile(Vector3(-0.5f, -0.5f, 0), Vector2(0, 1), Vector2(0, 1), color, 0);
        vertices[1] = VertexTile(Vector3(-0.5f, 0.5f, 0), Vector2(0, 0), Vector2(0, 0), color, 1);
        vertices[2] = VertexTile(Vector3(0.5f, -0.5f, 0), Vector2(1, 1), Vector2(1, 1), color, 2);
        vertices[3] = VertexTile(Vector3(0.5f, 0.5f, 0), Vector2(1, 0), Vector2(1, 0), color, 3);
    }

    //vertexBuffer
    {
        vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
    }

    //indices
    {
        indices = { 0, 1, 2, 2, 1, 3 };
    }

    //indexBuffer
    {
        indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
    }
    
    //LoadTexture
    {
        LoadTexture();
    }

    //UV
    {
        // 일반 모드: 전체 텍스처 사용
        startUV = Vector2(0, 0);
        endUV = Vector2(1, 1);
    }

    //UpdateUV
    {
        UpdateUV();
    }
}

Tile::Tile(const string& name, const Vector3& position, const Vector3& scale, float rotation, const wstring& tileSetPath, UINT columns, UINT rows, UINT tileIndex)
    : Drawable(name, position, scale, rotation, TransformType::Normal, ShaderPath + L"VertexTile.hlsl"), useTileSet(true), texturePath(tileSetPath), columns(columns), rows(rows), tileIndex(tileIndex), color(WHITE)
{
    //vertices
    {
        vertices.resize(4);
        // 초기 UV는 빈 값; UpdateUV()에서 계산됨

        vertices[0] = VertexTile(Vector3(-0.5f, -0.5f, 0), Vector2(), Vector2(), color, 0);
        vertices[1] = VertexTile(Vector3(-0.5f, 0.5f, 0), Vector2(), Vector2(), color, 1);
        vertices[2] = VertexTile(Vector3(0.5f, -0.5f, 0), Vector2(), Vector2(), color, 2);
        vertices[3] = VertexTile(Vector3(0.5f, 0.5f, 0), Vector2(), Vector2(), color, 3);
    }

    //vertexBuffer
    {
        vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
    }

    //indices
    {
        indices = { 0, 1, 2, 2, 1, 3 };
    }
    
    //indexBuffer
    {
        indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
    }

    //LoadTexture
    {
        LoadTexture();
    }

    //UpdateUV
    {
        UpdateUV();
    }

    //RenderStates
    {
        SetupRenderStates();
    }
}

Tile::~Tile() {}

void Tile::LoadTexture()
{
    if (texturePath.empty()) return;
    HRESULT hr = D3DX11CreateShaderResourceViewFromFile
    (
        DEVICE,
        texturePath.c_str(),
        nullptr,
        nullptr,
        srv.GetAddressOf(),
        nullptr
    );
    CHECK(hr);
}

void Tile::UpdateUV()
{
    if (useTileSet)
    {
        // 타일셋 모드: 전체 텍스처를 (0,0)-(1,1)로 가정
        float tileWidthUV = 1.0f / static_cast<float>(columns);
        float tileHeightUV = 1.0f / static_cast<float>(rows);
        UINT col = tileIndex % columns;
        UINT row = tileIndex / columns;

        vertices[0].uv = Vector2(col * tileWidthUV, (row + 1) * tileHeightUV);  // 좌 하단
        vertices[1].uv = Vector2(col * tileWidthUV, row * tileHeightUV);        // 좌 상단
        vertices[2].uv = Vector2((col + 1) * tileWidthUV, (row + 1) * tileHeightUV);  // 우 하단
        vertices[3].uv = Vector2((col + 1) * tileWidthUV, row * tileHeightUV);        // 우 상단

        // uv2는 동일하게 설정 (추후 추가 효과 가능)
        vertices[0].uv2 = vertices[0].uv;
        vertices[1].uv2 = vertices[1].uv;
        vertices[2].uv2 = vertices[2].uv;
        vertices[3].uv2 = vertices[3].uv;
    }
    else
    {
        vertices[0].uv = Vector2(0, 1);
        vertices[1].uv = Vector2(0, 0);
        vertices[2].uv = Vector2(1, 1);
        vertices[3].uv = Vector2(1, 0);
        vertices[0].uv2 = vertices[0].uv;
        vertices[1].uv2 = vertices[1].uv;
        vertices[2].uv2 = vertices[2].uv;
        vertices[3].uv2 = vertices[3].uv;
    }
    // 정점의 색상 적용
    for (auto& v : vertices) v.color = color;

    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = DC->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    CHECK(hr);
    memcpy(mapped.pData, vertices.data(), sizeof(VertexTile) * vertices.size());
    DC->Unmap(vertexBuffer->GetResource(), 0);
}

void Tile::SetupRenderStates()
{
    // Sampler
    {
        D3D11_SAMPLER_DESC desc;
        RenderStates::GetSamplerDesc(&desc);
        RenderStates::CreateSampler(&desc, &sampler[0]);
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        RenderStates::CreateSampler(&desc, &sampler[1]);
    }

    // Blend
    {
        D3D11_BLEND_DESC desc;
        RenderStates::GetBlendDesc(&desc);
        RenderStates::CreateBlend(&desc, &blend[0]);
        desc.RenderTarget[0].BlendEnable = true;
        RenderStates::CreateBlend(&desc, &blend[1]);
    }

    // ColorBuffer 생성
    colorBuffer = make_unique<ColorBuffer>();

    // InputLayout: 여기서는 VertexTile의 데이터 구조 대신 기존 VertexTexture 구조 사용 (필요하면 수정)
    inputLayout->Create(VertexTexture::ieDescs.data(), VertexTexture::ieDescs.size(), vertexShader->GetBlob());

    // Rasterizer State
    {
        D3D11_RASTERIZER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.CullMode = D3D11_CULL_BACK;
        desc.FillMode = D3D11_FILL_SOLID;
        desc.FrontCounterClockwise = false;
        HRESULT hr = DEVICE->CreateRasterizerState(&desc, &rs);
        CHECK(hr);
    }
}

void Tile::Update()
{
    BASE::Update();
}

void Tile::Render()
{
    BASE::Render();

    // 설정한 상태 적용
    DC->PSSetSamplers(0, 1, sampler[1].GetAddressOf());
    DC->OMSetBlendState(blend[1].Get(), nullptr, 0xFFFFFFFF);
    DC->RSSetState(rs.Get());

    // 텍스처 바인딩
    DC->PSSetShaderResources(0, 1, srv.GetAddressOf());
    DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Tile::SetTileIndex(UINT index)
{
    if (useTileSet)
    {
        tileIndex = index;
        UpdateUV();
    }
}
