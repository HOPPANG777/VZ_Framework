#pragma once
#include "Framework.h"

#include "AnimationClip.h"

class Animator
{
public:
    Animator();
    // 스마트 포인터로 관리하므로 특별히 리소스를 지울 필요가 없기에
    // 굳이 구현할 필요가 없으면 default로 둘 수 있음
    ~Animator();

    // 매 프레임마다 애니메이션 업데이트
    void Update();

    // 현재 실행중인 이미지 좌표값(UV) 반환
    Vector2 GetCurrentFrame() const { return currentFrame; }

    // 현재 실행중인 이미지 한 프레임의 UV 크기
    Vector2 GetTexelFrameSize() const
    {
        // currentClip이 항상 존재한다고 가정
        return currentClip->texelFrameSize;
    }

    // 현재 실행중인 이미지 SRV (COM 포인터)
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetCurrentSRV() const
    {
        return currentClip->srv;
    }

    // Animator가 실행할 클립(애니메이션) 추가
    // 스마트 포인터로 AnimationClip 관리
    void AddAnimClip(const std::shared_ptr<AnimationClip>& animClip);

    // 현재 실행시킬 클립 지정
    void SetCurrentAnimClip(const std::wstring& clipName);

    // 현재 실행중인 클립 이름
    std::wstring GetCurrentClipName() const
    {
        return currentClip ? currentClip->clipName : L"";
    }

    // 이미지 전환 속도 지정 (기본 1/15초)
    void SetPlayRate(float rate) { playRate = rate; }

private:
    // animClips에서 clipName에 해당하는 클립이 있는지 확인
    bool CheckExist(const std::wstring& clipName) const
    {
        return (animClips.find(clipName) != animClips.end());
    }

private:
    // 클립들을 shared_ptr로 보관
    std::unordered_map<std::wstring, std::shared_ptr<AnimationClip>> animClips;

    // 현재 실행중인 클립
    std::shared_ptr<AnimationClip> currentClip = nullptr;

    // 현재 그려야 할 프레임 인덱스/UV
    UINT currentFrameIndex = 0;
    Vector2 currentFrame = Values::ZeroVec2;

    // 애니메이션 플레이에 사용되는 시간 관리
    float deltaTime = 0.0f;
    float playRate = 1.0f / 15.0f; // 기본 15fps
};