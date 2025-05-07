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

    // 경과 시간이 animationTime을 넘으면 다음 프레임으로
    if (frameTime >= animationTime)
    {
        if (!currentClip->bReversed)
        {
            // 정방향 재생
            ++currentFrameIndex;
            if (currentFrameIndex >= currentClip->frameCount)
            {
                if (bLoop)
                {
                    // 반복 재생
                    currentFrameIndex = 0;
                }
                else
                {
                    // 한 번만 재생 → 마지막 프레임 유지 후 완료 플래그 설정
                    currentFrameIndex = currentClip->frameCount - 1;
                    bFinished = true;
                }
            }
        }
        else
        {
            // 역방향 재생
            if (currentFrameIndex == 0)
            {
                if (bLoop)
                {
                    // 반복 재생
                    currentFrameIndex = currentClip->frameCount - 1;
                }
                else
                {
                    // 한 번만 재생 → 첫 프레임에서 완료
                    bFinished = true;
                }
            }
            else
            {
                --currentFrameIndex;
            }
        }

        // 현재 프레임 갱신 및 시간 초기화
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
    // 이미 같은 클립이면 무시
    if (currentClip && currentClip->clipName == clipName)
    {
        return;
    }

    if (CheckExist(clipName))
    {
        currentClip = animClips.at(clipName);
        frameTime = 0.0f;
        bFinished = false; // 새로 시작할 땐 완료 상태 리셋

        if (currentClip->bReversed)
        {
            // 역재생이라면 마지막 프레임에서 시작
            currentFrameIndex = currentClip->frameCount - 1;
        }
        else
        {
            // 정방향이면 첫 프레임부터
            currentFrameIndex = 0;
        }

        currentFrame = currentClip->keyFrames[currentFrameIndex];
    }
}
