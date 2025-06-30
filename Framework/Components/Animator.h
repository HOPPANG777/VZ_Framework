#pragma once
// Animator.h
#include <unordered_map>
#include "Component.h"
#include "Utilities/AnimationClip.h"

// Animator가 Component를 상속받도록 변경
class Animator : public Component
{
public:
    // 생성자에서 Component 이름 "Animator" 지정
    Animator(const string& compName = "Animator")
        : Component(compName)
    {
    }

    virtual ~Animator() override = default;

    // Component의 순수 가상함수 구현
    virtual void Update() override;
    virtual void Render() override; // 보통은 비워두지만 필요시 구현

public:
    // ---------- 기존 Animator 멤버들 그대로 유지 ----------
    void AddAnimClip(const shared_ptr<AnimationClip>& animClip);
    void SetCurrentAnimClip(const wstring& clipName);

public:
    Vector2 GetCurrentFrame() const { return currentFrame; }
    Vector2 GetTexelFrameSize() const
    {
        return currentClip ? currentClip->texelFrameSize : Values::ZeroVec2;
    }

    ComPtr<ID3D11ShaderResourceView> GetCurrentSRV() const
    {
        return currentClip ? currentClip->srv : nullptr;
    }

public:
    void SetAnimationTime(float _animationTime) { animationTime = _animationTime; }
    std::wstring GetCurrentClipName() const
    {
        return currentClip ? currentClip->clipName : L"";
    }

public:
    // ---------- 루프 옵션 추가 ----------
    // true = 계속 루프, false = 한 번만 재생
    void SetLoop(bool loop) { bLoop = loop; if (loop) bFinished = false; }
    bool IsFinished() const { return bFinished; }

public:
    // ▶ 새로 추가할 재생 속도 / 시간 제어 함수들
    // playbackSpeed:
    //   > 0 → 정방향 재생 (1.0f = 기본 속도, 2.0f = 두 배속 등)
    //   < 0 → 역방향 재생 (-1.0f = 한 배속 역방향)
    //   = 0 → 정지 (프레임 멈춤)
    void                    SetPlaybackSpeed(float speed);
    float                   GetPlaybackSpeed() const { return playbackSpeed; }

public:
    // normalizedTime: 0.0f ~ 1.0f 사이 값으로 전체 애니메이션의 재생 위치를 지정
    //   0.0f → 첫 프레임, 1.0f → 마지막 프레임
    void                    SetNormalizedTime(float t);
    float                   GetNormalizedTime() const;

public:
    // 현재 재생 속도(playbackSpeed)에 따라,
    // 정방향이면 마지막 프레임(1.0) 또는 역방향이면 첫 프레임(0.0)에 도달했는지 여부
    bool                    IsAnimationFinished() const;

private:
    bool CheckExist(const std::wstring& clipName) const;

private:
    // ---------- 애니메이션 클립 자료구조 ----------
    unordered_map<wstring, shared_ptr<AnimationClip>> animClips;
    shared_ptr<AnimationClip> currentClip = nullptr;

    // 애니메이션 재생 관련
    UINT    currentFrameIndex = 0;
    Vector2 currentFrame = Values::ZeroVec2;
    float   frameTime = 0.0f;
    float   animationTime = 1.0f / 15.0f; // 기본 15FPS

    // 루프 제어 플래그
    bool bLoop = true;   // 기본은 루프 모드
    bool bFinished = false;  // 한 번만 재생 후 완료 여부

    // ▶ 새로 추가할 멤버 변수들
    float playbackSpeed = 1.0f;     // 1.0f = 기본 정방향, -1.0f = 기본 역방향, 0.0f = 정지
    float normalizedTime = 0.0f;    // 0.0f ~ 1.0f (0=첫 프레임, 1=마지막 프레임)
};
