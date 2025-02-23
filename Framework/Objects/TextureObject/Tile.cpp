#include "Framework.h"
#include "Tile.h"

Tile::Tile(const string& name, const Vector3& position, const Vector3& scale, float rotation, const wstring& texturePath)
    : Drawable(name, position, scale, rotation, TransformType::Normal, ShaderPath + L"VertexTile.hlsl"), useTileSet(false), texturePath(texturePath), columns(1), rows(1), tileIndex(0)
{
    //vertices
    {
        vertices.resize(4);

        // 정사각형 형태의 타일: 4개의 정점, 중심 기준
        vertices[0] = VertexTile(Vector3(-0.5f, -0.5f, 0), Vector2(0, 1), Vector2(0, 1), Color(1, 1, 1, 1), 0);
        vertices[1] = VertexTile(Vector3(-0.5f, 0.5f, 0), Vector2(0, 0), Vector2(0, 0), Color(1, 1, 1, 1), 1);
        vertices[2] = VertexTile(Vector3(0.5f, -0.5f, 0), Vector2(1, 1), Vector2(1, 1), Color(1, 1, 1, 1), 2);
        vertices[3] = VertexTile(Vector3(0.5f, 0.5f, 0), Vector2(1, 0), Vector2(1, 0), Color(1, 1, 1, 1), 3);
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

    // Texture load and UV setting
    {
        LoadTexture();
        UpdateUV();
    }
    
}

Tile::Tile(const string& name, const Vector3& position, const Vector3& scale, float rotation, const wstring& tileSetPath, UINT columns, UINT rows, UINT tileIndex)
    : Drawable(name, position, scale, rotation, TransformType::Normal, ShaderPath + L"VertexTile.hlsl"), useTileSet(true), texturePath(tileSetPath), columns(columns), rows(rows), tileIndex(tileIndex)
{

    //vertices
    {
        vertices.resize(4);

        // 초기 정점 위치는 동일하나, UV는 나중에 계산
        vertices[0] = VertexTile(Vector3(-0.5f, -0.5f, 0), Vector2(), Vector2(), Color(1, 1, 1, 1), 0);
        vertices[1] = VertexTile(Vector3(-0.5f, 0.5f, 0), Vector2(), Vector2(), Color(1, 1, 1, 1), 1);
        vertices[2] = VertexTile(Vector3(0.5f, -0.5f, 0), Vector2(), Vector2(), Color(1, 1, 1, 1), 2);
        vertices[3] = VertexTile(Vector3(0.5f, 0.5f, 0), Vector2(), Vector2(), Color(1, 1, 1, 1), 3);
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
    
    // Texture load and UV setting
    {
        LoadTexture();
        UpdateUV();
    }

    // Sampler, Blend
    {
        SetupRenderStates();
    }

}

Tile::~Tile()
{
    // 스마트 포인터가 자동 소멸합니다.
}

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

        vertices[0].uv = Vector2(col * tileWidthUV, (row + 1) * tileHeightUV);    // 좌 하단
        vertices[1].uv = Vector2(col * tileWidthUV, row * tileHeightUV);          // 좌 상단
        vertices[2].uv = Vector2((col + 1) * tileWidthUV, (row + 1) * tileHeightUV);  // 우 하단
        vertices[3].uv = Vector2((col + 1) * tileWidthUV, row * tileHeightUV);        // 우 상단

        // uv2를 활용해 추가 효과를 줄 수 있습니다.
        // 여기서는 기본적으로 동일한 값으로 설정합니다.
        vertices[0].uv2 = vertices[0].uv;
        vertices[1].uv2 = vertices[1].uv;
        vertices[2].uv2 = vertices[2].uv;
        vertices[3].uv2 = vertices[3].uv;
    }
    else
    {
        // 개별 타일 모드: 전체 텍스처 사용
        vertices[0].uv = Vector2(0, 1);
        vertices[1].uv = Vector2(0, 0);
        vertices[2].uv = Vector2(1, 1);
        vertices[3].uv = Vector2(1, 0);

        vertices[0].uv2 = vertices[0].uv;
        vertices[1].uv2 = vertices[1].uv;
        vertices[2].uv2 = vertices[2].uv;
        vertices[3].uv2 = vertices[3].uv;
    }

    // 버텍스 버퍼 업데이트
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

        // POINT 필터 (선형보간 대신)
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
    {
        colorBuffer = make_unique<ColorBuffer>();
    }

    // InputLayout 재설정: VertexTile를 사용하므로 VertexTile::descs를 사용할 수도 있으나,
    // 여기서는 기존 VertexTexture와 동일한 구조를 사용한다고 가정
    inputLayout->Create(VertexTexture::ieDescs.data(), VertexTexture::ieDescs.size(), vertexShader->GetBlob());

    // Rasterizer State 설정
    {
        D3D11_RASTERIZER_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
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
    // 동적 타일 변경이나 애니메이션 로직이 필요한 경우 여기서 처리합니다.
}

void Tile::Render()
{
    BASE::Render();

    // 설정한 상태를 적용 (예: Sampler, Blend, Rasterizer)
    DC->PSSetSamplers(0, 1, sampler[1].GetAddressOf());
    DC->OMSetBlendState(blend[1].Get(), nullptr, 0xFFFFFFFF);
    DC->RSSetState(rs.Get());

    // 텍스처 SRV 바인딩 (픽셀 셰이더 슬롯 0)
    DC->PSSetShaderResources(0, 1, srv.GetAddressOf());

    // DrawCall 호출
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

