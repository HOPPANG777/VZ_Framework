#include "Framework.h"
#include "AnimationRect.h"

AnimationRect::AnimationRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path)
    : Drawable("AnimationRect", position, scale, rotation, ShaderPath + L"VertexTexture.hlsl")
{
    // Vertex 설정
    {
        vertices.assign(4, VertexTexture());

        // 좌 하단
        vertices[0].position = { -0.5f, -0.5f, 0 };
        vertices[0].uv = Vector2(0, 1);

        // 좌 상단
        vertices[1].position = { -0.5f, 0.5f, 0 };
        vertices[1].uv = Vector2(0, 0);

        // 우 하단
        vertices[2].position = { 0.5f, -0.5f, 0 };
        vertices[2].uv = Vector2(1, 1);

        // 우 상단
        vertices[3].position = { 0.5f, 0.5f, 0 };
        vertices[3].uv = Vector2(1, 0);
    }

    // VertexBuffer
    {
        vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
    }

    // Index 설정
    {
        indices = { 0, 1, 2, 2, 1, 3 };
    }

    // IndexBuffer
    {
        indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
    }

    // InputLayout
    {
        inputLayout->Create(VertexTexture::ieDescs.data(), VertexTexture::ieDescs.size(), vertexShader->GetBlob());
    }

    // Rasterizer
    {
        D3D11_RASTERIZER_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

        desc.CullMode = D3D11_CULL_BACK;
        desc.FillMode = D3D11_FILL_SOLID;
        desc.FrontCounterClockwise = false;

        HRESULT hr = DEVICE->CreateRasterizerState(&desc, &rs);
        CHECK(hr);
    }

    // SRV 로드 (애니메이션 아틀라스를 통째로)
    {
        HRESULT hr = D3DX11CreateShaderResourceViewFromFile(
            DEVICE,
            path.c_str(),
            nullptr,
            nullptr,
            srv.GetAddressOf(),
            nullptr
        );
        CHECK(hr);
    }

    // Sampler
    {
        D3D11_SAMPLER_DESC desc;
        RenderStates::GetSamplerDesc(&desc);
        RenderStates::CreateSampler(&desc, &sampler[0]);

        // 예: POINT 필터
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

    // 여기서 Animator를 생성
    animator = std::make_shared<Animator>();

    //// AnimationClip 생성
    //{
    //    // Texture2D를 shared_ptr로 관리 (Animator, AnimationClip과 동일 생명주기 사용)
    //    shared_ptr<Texture2D> srcTex = std::make_shared<Texture2D>(path);

    //    // 예: 아래 두 클립을 만든다고 가정
    //    auto RunD = std::make_shared<AnimationClip>(
    //        L"RunD",
    //        srcTex.get(),
    //        4,
    //        1,
    //        Values::ZeroVec2, // (0,0)에서 시작
    //        Vector2(srcTex->GetWidth() * 0.5f, srcTex->GetHeight() * 0.125f)
    //    );

    //    auto RunL = std::make_shared<AnimationClip>(
    //        L"RunL",
    //        srcTex.get(),
    //        4,
    //        1,
    //        Vector2(0, srcTex->GetHeight() * 0.125f * 2),
    //        Vector2(srcTex->GetWidth() * 0.5f, srcTex->GetHeight() * 0.125f * 3)
    //    );

    //    // Animator에 추가
    //    animator->AddAnimClip(RunD);
    //    animator->AddAnimClip(RunL);

    //    // 현재 재생할 기본 클립
    //    animator->SetCurrentAnimClip(L"RunL");
    //}
}

AnimationRect::~AnimationRect()
{
}

void AnimationRect::Update()
{
    //// 키 입력에 따라 클립 교체
    //if (Keyboard::Get()->Press('A'))
    //{
    //    animator->SetCurrentAnimClip(L"RunL");
    //}
    //else if (Keyboard::Get()->Press('S'))
    //{
    //    animator->SetCurrentAnimClip(L"RunD");
    //}

    // 애니메이션 진행
    animator->Update();

    // 버텍스 UV 갱신
    DC->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subRe);
    {
        Vector2 currentFrame = animator->GetCurrentFrame();
        Vector2 frameSize = animator->GetTexelFrameSize();

        // 좌 하단
        vertices[0].uv.x = currentFrame.x;
        vertices[0].uv.y = currentFrame.y + frameSize.y;

        // 좌 상단
        vertices[1].uv.x = currentFrame.x;
        vertices[1].uv.y = currentFrame.y;

        // 우 하단
        vertices[2].uv.x = currentFrame.x + frameSize.x;
        vertices[2].uv.y = currentFrame.y + frameSize.y;

        // 우 상단
        vertices[3].uv.x = currentFrame.x + frameSize.x;
        vertices[3].uv.y = currentFrame.y;

        memcpy(subRe.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
    }
    DC->Unmap(vertexBuffer->GetResource(), 0);

    // Drawable Update (행렬 계산 등)
    BASE::Update();
}

void AnimationRect::Render()
{
    BASE::Render();

    // Animator가 가진 SRV로 교체해서 그림
    srv = animator->GetCurrentSRV();

    // Sampler / RS / Blend State 세팅
    DC->PSSetSamplers(0, 1, sampler[1].GetAddressOf());
    DC->PSSetShaderResources(0, 1, srv.GetAddressOf());
    DC->RSSetState(rs.Get());
    DC->OMSetBlendState(blend[1].Get(), nullptr, 0xFFFFFFFF);

    // DrawCall
    DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
