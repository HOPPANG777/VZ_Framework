// Animator.cpp
#include "Framework.h"
#include "Animator.h"

void Animator::Update()
{
    // 재생할 클립이 없거나, 한 번만 재생 후 완료된 상태면 리턴
    if (!currentClip || (bFinished && !bLoop))
    {
        return;
    }

    // ▶ 1) playbackSpeed == 0 이면 정지 상태이므로 아무것도 하지 않음
    if (playbackSpeed == 0.0f)
    {
        return;
    }

    // ▶ 2) normalizedTime 업데이트
    double dt = Time::Get()->GetDeltaTime();

    if (currentClip->frameCount > 0 && animationTime > 0.0f)
    {
        float clipDuration = animationTime * static_cast<float>(currentClip->frameCount);

        normalizedTime += (playbackSpeed * dt) / clipDuration;

        // ▶ std::clamp 대신 수동 클램프
        if (normalizedTime < 0.0f)
        {
            normalizedTime = 0.0f;
        }
        else if (normalizedTime > 1.0f)
        {
            normalizedTime = 1.0f;
        }
    }

    // ▶ 3) normalizedTime에 따라 currentFrameIndex 계산
    if (currentClip->frameCount > 0)
    {
        float rawIndex = normalizedTime * static_cast<float>(currentClip->frameCount - 1);
        currentFrameIndex = static_cast<UINT>(rawIndex + 0.5f);

        if (currentFrameIndex >= currentClip->frameCount)
        {
            currentFrameIndex = currentClip->frameCount - 1;
        }
    }

    // ▶ 4) bLoop / bFinished 처리
    if (playbackSpeed > 0.0f)
    {
        if (normalizedTime >= 1.0f)
        {
            if (bLoop)
            {
                normalizedTime = 0.0f;
                currentFrameIndex = 0;
            }
            else
            {
                currentFrameIndex = currentClip->frameCount - 1;
                bFinished = true;
                playbackSpeed = 0.0f;
            }
        }
    }
    else if (playbackSpeed < 0.0f)
    {
        if (normalizedTime <= 0.0f)
        {
            if (bLoop)
            {
                normalizedTime = 1.0f;
                currentFrameIndex = currentClip->frameCount - 1;
            }
            else
            {
                currentFrameIndex = 0;
                bFinished = true;
                playbackSpeed = 0.0f;
            }
        }
    }

    // ▶ 5) 실제 렌더러에게 currentFrame을 넘김
    currentFrame = currentClip->keyFrames[currentFrameIndex];
}

void Animator::Render()
{
    // 특별히 렌더할 게 없지만, 디버그용 코드가 필요하면 여기에 작성
}

void Animator::AddAnimClip(const std::shared_ptr<AnimationClip>& animClip)
{
    animClips[animClip->clipName] = animClip;
}

bool Animator::CheckExist(const std::wstring& clipName) const
{
    return (animClips.find(clipName) != animClips.end());
}

void Animator::SetCurrentAnimClip(const std::wstring& clipName)
{
    // 이미 같은 클립이면 무시
    if (currentClip && currentClip->clipName == clipName)
    {
        return;
    }

    if (CheckExist(clipName))
    {
        currentClip = animClips.at(clipName);

        // 기존 frameTime 초기화 (이제 normalizedTime 기반으로 변경되므로 큰 의미 없음)
        frameTime = 0.0f;
        normalizedTime = 0.0f;
        bFinished = false;
        playbackSpeed = 1.0f;    // 기본 정방향 1배속

        if (currentClip->bReversed)
        {
            // 만약 클립 자체가 bReversed=true인 경우 역방향 기본 시작
            normalizedTime = 1.0f;
            playbackSpeed = -1.0f;
            currentFrameIndex = currentClip->frameCount - 1;
        }
        else
        {
            currentFrameIndex = 0;
        }

        currentFrame = currentClip->keyFrames[currentFrameIndex];
    }
    else
    {
        assert(false && "Animator::SetCurrentAnimClip: Clip not found");
    }
}

// ▶ 새로 추가: 재생 속도를 설정
void Animator::SetPlaybackSpeed(float speed)
{
    playbackSpeed = speed;
}

// ▶ 새로 추가: normalizedTime 설정
void Animator::SetNormalizedTime(float t)
{
    // ▶ 수동 클램프로 0.0f ~ 1.0f 범위 유지
    if (t < 0.0f)
    {
        normalizedTime = 0.0f;
    }
    else if (t > 1.0f)
    {
        normalizedTime = 1.0f;
    }
    else
    {
        normalizedTime = t;
    }

    if (currentClip && currentClip->frameCount > 0)
    {
        float rawIndex = normalizedTime * static_cast<float>(currentClip->frameCount - 1);
        currentFrameIndex = static_cast<UINT>(rawIndex + 0.5f);

        if (currentFrameIndex >= currentClip->frameCount)
        {
            currentFrameIndex = currentClip->frameCount - 1;
        }

        currentFrame = currentClip->keyFrames[currentFrameIndex];
    }
}

float Animator::GetNormalizedTime() const
{
    return normalizedTime;
}

// ▶ 새로 추가: 재생 속도에 따라 끝/처음에 도달했는지 확인
bool Animator::IsAnimationFinished() const
{
    if (!currentClip)
    {
        return true;
    }

    if (playbackSpeed > 0.0f)
    {
        return (normalizedTime >= 1.0f);
    }
    else if (playbackSpeed < 0.0f)
    {
        return (normalizedTime <= 0.0f);
    }

    // playbackSpeed == 0.0f → 이미 정지된 상태라면 bFinished 값을 리턴
    return bFinished;
}