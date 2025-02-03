#include "Framework.h"
#include "Animator.h"

void Animator::Update()
{
    // currentClip이 없으면 리턴
    if (!currentClip) return;

    // 경과 시간이 playRate를 넘으면 다음 프레임
    if (frameTime >= animationTime)
    {
        if (!currentClip->bReversed) // 정방향
        {
            ++currentFrameIndex;
            if (currentFrameIndex == currentClip->frameCount) currentFrameIndex = 0;
        }
        else // 역재생
        {
            if (currentFrameIndex == 0) currentFrameIndex = currentClip->frameCount;
            --currentFrameIndex;
        }

        currentFrame = currentClip->keyFrames[currentFrameIndex];
        frameTime = 0.0f;
    }
    else
    {
        frameTime += DELTA;
    }
}

void Animator::Render()
{
    // Animator는 특별히 렌더할 게 없지만,
    // 디버그 정보를 그리고 싶다면 여기에 작성할 수 있음.
}

void Animator::AddAnimClip(const shared_ptr<AnimationClip>& animClip)
{
    // 같은 이름이 들어오면 덮어씌움
    animClips[animClip->clipName] = animClip;
}

bool Animator::CheckExist(const wstring& clipName) const
{
    return (animClips.find(clipName) != animClips.end());
}

void Animator::SetCurrentAnimClip(const wstring& clipName)
{
    if (currentClip && currentClip->clipName == clipName)
        return;

    if (CheckExist(clipName))
    {
        currentClip = animClips.at(clipName);
        frameTime = 0.0f;

        // 역재생이라면 마지막 프레임에서 시작
        if (currentClip->bReversed) currentFrameIndex = currentClip->frameCount - 1;
        else currentFrameIndex = 0;

        currentFrame = currentClip->keyFrames[currentFrameIndex];
    }
}