#pragma once
#include <string>

#include "Renders/RenderResources/Texture2D.h"
#include "DirectHelper.h"

class AnimationClip
{
public:
    friend class Animator;

    // AnimationClip 생성자
    // SRV는 Texture2D로부터 얻어오고, 계산된 UV 프레임들을 저장
    AnimationClip
    (
        const std::wstring& clipName,
        Texture2D* srcTex,
        UINT columns,    // 가로 프레임 수
        UINT rows,       // 세로 프레임 수
        Vector2 startPos,
        Vector2 endPos,
        bool bReversed = false
        //float playRate = 1.0f / 15.0f  // ★ 디폴트 1/15초로 설정
    );

private:
    std::wstring clipName = L"";
    std::vector<Vector2> keyFrames;          // 각 프레임의 UV(텍셀) 좌표
    UINT frameCount = 0;

    // 원본에서는 Raw Pointer였지만, 이제는 ComPtr
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;

    Vector2 texelFrameSize = Values::ZeroVec2;
    bool bReversed = false;

    // ★ 추가: 이 애니메이션의 재생 속도
    //float playRate = 0.0f;   // 기본 1/15초 (15FPS)
};
