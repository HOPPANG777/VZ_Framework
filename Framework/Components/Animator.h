#pragma once

// Animator가 Component를 상속받도록 변경
class Animator : public Component
{
public:
    // 생성자에서 Component 이름 "Animator" 지정
    Animator(const string& compName = "Animator") : Component(compName) {}
    virtual ~Animator() override = default;

    // Component의 순수 가상함수 구현
    virtual void Update() override;
    virtual void Render() override; // 보통은 비워두지만 필요시 구현

public:
    // ---------- 기존 Animator 멤버들 그대로 유지 ----------
    void AddAnimClip(const shared_ptr<AnimationClip>& animClip);
    void SetCurrentAnimClip(const wstring& clipName);

    Vector2 GetCurrentFrame() const { return currentFrame; }
    Vector2 GetTexelFrameSize() const
    {
        return currentClip ? currentClip->texelFrameSize : Values::ZeroVec2;
    }

    ComPtr<ID3D11ShaderResourceView> GetCurrentSRV() const
    {
        return currentClip ? currentClip->srv : nullptr;
    }

    void SetPlayRate(float rate) { playRate = rate; }
    std::wstring GetCurrentClipName() const
    {
        return currentClip ? currentClip->clipName : L"";
    }

private:
    bool CheckExist(const std::wstring& clipName) const;

private:
    // ---------- 애니메이션 클립 자료구조 ----------
    unordered_map<wstring, shared_ptr<AnimationClip>> animClips;
    shared_ptr<AnimationClip> currentClip = nullptr;

    // 애니메이션 재생 관련
    UINT currentFrameIndex = 0;
    Vector2 currentFrame = Values::ZeroVec2;
    float deltaTime = 0.0f;
    float playRate = 1.0f / 15.0f; // 기본 15FPS
    // ----------------------------------------------
};