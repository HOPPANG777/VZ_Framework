#pragma once

class Texture2D;

// 실질적인 애니메이션 이미지 분할 모음
class AnimationClip
{
public:
    AnimationClip(
        const wstring& clipName,
        shared_ptr<Texture2D> srcTex,
        UINT frameCount,
        Vector2 startPos,
        Vector2 endPos,
        bool bReversed = false);

    const wstring& GetClipName() const { return clipName; }
    ID3D11ShaderResourceView* GetSRV() const { return srv.Get(); }
    const Vector2& GetTexelFrameSize() const { return texelFrameSize; }
    const vector<Vector2>& GetKeyFrames() const { return keyFrames; }
    UINT GetFrameCount() const { return frameCount; }
    bool IsReversed() const { return bReversed; }

private:
    wstring clipName = L"";
    vector<Vector2> keyFrames;  // 애니메이션 시작 위치들 저장
    UINT frameCount = 0;
    ComPtr<ID3D11ShaderResourceView> srv = nullptr;
    Vector2 texelFrameSize = Values::ZeroVec2;
    bool bReversed = false;
};

// 애니메이션 클립 실행 도구
class Animator
{
public:
    Animator();
    ~Animator();

    void Update();

    // 현재 실행중인 이미지 좌표값 반환
    Vector2 GetCurrentFrame() const { return currentFrame; }
    // 현재 실행중인 이미지 uv값 반환
    Vector2 GetTexelFrameSize() const { return currentClip->GetTexelFrameSize(); }
    // 현재 실행중인 이미지 srv 반환
    ID3D11ShaderResourceView* GetCurrentSRV() const { return currentClip->GetSRV(); }

    // animClips에 클립 추가
    void AddAnimClip(shared_ptr<AnimationClip> animClip);
    // 지금 실행시킬 클립 지정
    void SetCurrentAnimClip(const wstring& clipName);

    // 현재 실행중인 클립 이름 반환
    wstring GetCurrentClipName() const { return currentClip->GetClipName(); }
    // 실행중인 애니메이션 속도 조절
    void SetPlayRate(float playRate) { this->playRate = playRate; }

private:
    bool CheckExist(const wstring& clipName) const
    {
        return animClips.find(clipName) != animClips.end();
    }

    unordered_map<wstring, shared_ptr<AnimationClip>> animClips;
    shared_ptr<AnimationClip> currentClip = nullptr;
    UINT currentFrameIndex = 0;
    Vector2 currentFrame = Values::ZeroVec2;

    float deltaTime = 0.0f;

    float playRate = 1.0f / 15.0f;  // 이미지 전환 속도
};