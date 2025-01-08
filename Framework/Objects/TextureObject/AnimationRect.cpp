#include "Framework.h"
#include "AnimationRect.h"

AnimationRect::AnimationRect(const Vector3& position, const Vector3& scale, const float rotation, const wstring path)
	: TextureRect(position, scale, rotation, ShaderPath + L"VertexTexture.hlsl")
{
	animator = make_shared<Animator>();

	{
        // Texture2D를 unique_ptr로 관리
        shared_ptr<Texture2D> srcTex = make_unique<Texture2D>(path);

        // AnimationClip은 shared_ptr로 관리
        shared_ptr<AnimationClip> RunD = make_shared<AnimationClip>(
            L"RunD",                              // 애니메이션 이름
            srcTex.get(),                         // 애니메이션에 사용할 Texture2D
            4,                                    // 4칸 사용
            Values::ZeroVec2,                     // 초기 위치
            Vector2(srcTex->GetWidth() * 0.5f,    // 마지막 위치(자를 위치)
                srcTex->GetHeight() * 0.125f));

        shared_ptr<AnimationClip> RunL = make_shared<AnimationClip>(
            L"RunL",                              // 애니메이션 이름
            srcTex.get(),                         // 애니메이션에 사용할 Texture2D
            4,                                    // 4칸 사용
            Vector2(0, srcTex->GetHeight() * 0.125f * 2),
            Vector2(srcTex->GetWidth() * 0.5f, srcTex->GetHeight() * 0.125f * 3)); // 마지막 위치(자를 위치)

        // Animator에 AnimationClip 추가
        animator->AddAnimClip(RunD);
        animator->AddAnimClip(RunL);

        // srcTex는 고유 소유권을 가진 unique_ptr이므로 명시적으로 삭제할 필요가 없음
	}

    animator->SetCurrentAnimClip(L"RunL");

    // Sampler
    {
        D3D11_SAMPLER_DESC desc;
        RenderStates::GetSamplerDesc(&desc);
        RenderStates::CreateSampler(&desc, &sampler[0]);

        // 선형보간
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
}

AnimationRect::~AnimationRect() {}

void AnimationRect::Update()
{
    if (Keyboard::Get()->Press('A'))
    {
        animator->SetCurrentAnimClip(L"RunL");
    }

    else if (Keyboard::Get()->Press('S'))
    {
        animator->SetCurrentAnimClip(L"RunD");
    }

    animator->Update();

    DC->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subRe);
    {
        // 왼쪽 아래 0 , 1
        vertices[0].uv.x = animator->GetCurrentFrame().x;
        vertices[0].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

        // 오른쪽 위 1 , 0
        vertices[1].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
        vertices[1].uv.y = animator->GetCurrentFrame().y;

        // 오른쪽 아래 1 , 1
        vertices[2].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
        vertices[2].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

        // 좌 상단  0 , 0
        vertices[3].uv.x = animator->GetCurrentFrame().x;
        vertices[3].uv.y = animator->GetCurrentFrame().y;

        memcpy(subRe.pData, vertices.data(), sizeof(vertices[0]) * vertices.size()); 
    }
    DC->Unmap(vertexBuffer->GetResource(), 0);

    BASE::Update();
}

void AnimationRect::Render()
{
    srv = animator->GetCurrentSRV();

    BASE::Render();
}
