#include "Framework.h"
#include "AnimationRect.h"

AnimationRect::AnimationRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path)
    : Drawable("AnimationRect", position, scale, rotation, TransformType::Normal, ShaderPath + L"VertexTexture.hlsl")
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

        HRESULT hr = DEVICE->CreateRasterizerState(&desc, rs.GetAddressOf());
        CHECK(hr);
    }

    // SRV 로드 (애니메이션 아틀라스를 통째로)
    {
        HRESULT hr = D3DX11CreateShaderResourceViewFromFile
        (
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
        RenderStates::CreateSampler(&desc, sampler[0].GetAddressOf());

        // 예: POINT 필터
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        RenderStates::CreateSampler(&desc, sampler[1].GetAddressOf());
    }

    // Blend
    {
        D3D11_BLEND_DESC desc;
        RenderStates::GetBlendDesc(&desc);
        RenderStates::CreateBlend(&desc, blend[0].GetAddressOf());

        desc.RenderTarget[0].BlendEnable = true;
        RenderStates::CreateBlend(&desc, blend[1].GetAddressOf());
    }

    // 4) Animator 컴포넌트를 등록
    {
        const std::shared_ptr<Animator>& animatorComp = make_shared<Animator>("Animator");
        AddComponent(animatorComp);

        // ※ 여기서 애니메이션 클립들을 등록할 수도 있음
        // 예: 
        //   auto runClip = std::make_shared<AnimationClip>(L"Run", ... );
        //   animatorComp->AddAnimClip(runClip);
        //   animatorComp->SetCurrentAnimClip(L"Run");
    }

    // 5) ColorBuffer 생성
    {
        colorBuffer = make_unique<ColorBuffer>();
    }

    #pragma region OldTestClip
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
    #pragma endregion
}

AnimationRect::~AnimationRect()
{
}

void AnimationRect::Update()
{
    #pragma region OldTestClipControll
    //// 키 입력에 따라 클립 교체
    //if (Keyboard::Get()->Press('A'))
    //{
    //    animator->SetCurrentAnimClip(L"RunL");
    //}
    //else if (Keyboard::Get()->Press('S'))
    //{
    //    animator->SetCurrentAnimClip(L"RunD");
    //}
    #pragma endregion

     // 1) Animator 가져오기 & 애니메이션 Update
    const auto& animator = GetComponent<Animator>("Animator");
    if (animator)
    {
        animator->Update();

        // 2) 애니메이션에 따른 UV 갱신
        DC->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subRe);
        {
            Vector2 framePos = animator->GetCurrentFrame();
            Vector2 frameSize = animator->GetTexelFrameSize();

            // 좌 하단
            vertices[0].uv.x = framePos.x;
            vertices[0].uv.y = framePos.y + frameSize.y;

            // 좌 상단
            vertices[1].uv.x = framePos.x;
            vertices[1].uv.y = framePos.y;

            // 우 하단
            vertices[2].uv.x = framePos.x + frameSize.x;
            vertices[2].uv.y = framePos.y + frameSize.y;

            // 우 상단
            vertices[3].uv.x = framePos.x + frameSize.x;
            vertices[3].uv.y = framePos.y;

            memcpy(subRe.pData, vertices.data(), sizeof(VertexTexture) * vertices.size());
        }
        DC->Unmap(vertexBuffer->GetResource(), 0);

        // SRV 갱신 (Animator가 현재 재생중인 클립의 SRV)
        srv = animator->GetCurrentSRV();
    }

    // Drawable Update (행렬 계산 등)
    BASE::Update();
}

void AnimationRect::Render()
{
    BASE::Render();

    // 먼저, 픽셀 셰이더의 상수 버퍼 슬롯(예: b2)에 ColorBuffer를 업데이트 및 바인딩합니다.
    colorBuffer->SetPSBuffer(2);  // b2 슬롯에 바인딩 (셰이더 코드와 일치해야 함)

    // Sampler / RS / Blend State 세팅
    DC->PSSetSamplers(0, 1, sampler[1].GetAddressOf());

    DC->PSSetShaderResources(0, 1, srv.GetAddressOf());

    DC->RSSetState(rs.Get());

    DC->OMSetBlendState(blend[1].Get(), nullptr, 0xFFFFFFFF);

    // DrawCall
    DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
