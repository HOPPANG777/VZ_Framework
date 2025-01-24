#include "stdafx.h"
#include "JumpHoppang.h"

JumpHoppang::JumpHoppang(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path)
    : AnimationRect(position, scale, rotation, path)
{
    // "path"로부터 Texture2D 로드 & Animator 생성은 
    // AnimationRect 생성자에서 이미 처리됨

    // 이제 로딩 아이콘에 맞는 애니메이션 클립을 등록
    // 예: 가로 7프레임
    //    한 프레임 크기는 50x50, 총 이미지(350×50)라 가정
    auto tex = make_shared<Texture2D>(path);

    // AnimationClip 생성 (이름: "Loading")
    // - frameCount : 7
    // - startPos   : (0, 0)
    // - endPos     : (350, 50)  (tex->GetWidth(), tex->GetHeight() 라고 가정)
    auto loadingClip = make_shared<AnimationClip>(
        L"Loading",      // clipName
        tex.get(),             // Texture2D (shared_ptr)
        7,               // frameCount
        1,
        Values::ZeroVec2,
        Vector2((float)tex->GetWidth(), (float)tex->GetHeight()),
        false            // bReversed?
    );

    // AnimationRect 내부의 animator 멤버에 추가
    GetAnimator()->AddAnimClip(loadingClip);
    // 기본 클립으로 "Loading" 재생
    GetAnimator()->SetCurrentAnimClip(L"Loading");

    GetAnimator()->SetPlayRate(1.0f / 7.5f);
}

JumpHoppang::~JumpHoppang()
{
}

void JumpHoppang::Update()
{
    BASE::Update();
}

void JumpHoppang::Render()
{
    BASE::Render();
}
