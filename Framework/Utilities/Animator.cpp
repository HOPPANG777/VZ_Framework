#include "Framework.h"
#include "Animator.h"

Animator::Animator() {}

Animator::~Animator() {}

void Animator::Update()
{
    // 경과 시간이 playRate보다 커지면 다음 프레임으로 넘어감
    if (deltaTime >= playRate)
    {
        // 정방향 재생(bReversed == false)
        if (currentClip->bReversed == false)
        {
            ++currentFrameIndex;
            if (currentFrameIndex == currentClip->frameCount)
                currentFrameIndex = 0;

            currentFrame = currentClip->keyFrames[currentFrameIndex];
        }
        else // 역재생
        {
            // 역재생 시에는 인덱스를 감소
            if (currentFrameIndex == 0)
                currentFrameIndex = currentClip->frameCount;

            --currentFrameIndex;
            currentFrame = currentClip->keyFrames[currentFrameIndex];
        }

        deltaTime = 0.0f;
    }
    else
    {
        // playRate 이내라면 계속 시간을 누적
        deltaTime += DELTA;
    }
}

// 애니메이터에 클립을 추가
void Animator::AddAnimClip(const std::shared_ptr<AnimationClip>& animClip)
{
    // 중복 삽입 시 덮어쓰거나, 혹은 무시하는 로직은 선택
    // 여기서는 같은 이름이 들어오면 덮어쓰는 것으로 처리
    animClips[animClip->clipName] = animClip;
}

// 특정 이름의 클립을 현재 실행 클립으로 설정
void Animator::SetCurrentAnimClip(const std::wstring& clipName)
{
    // 현재 클립과 요청한 클립이 같다면 교체 X
    if (currentClip && (currentClip->clipName == clipName))
        return;

    // 클립이 존재한다면 교체
    if (CheckExist(clipName))
    {
        currentClip = animClips[clipName];

        // 시작 시점마다 시간 초기화
        deltaTime = 0.0f;

        // 역재생이라면 마지막 프레임에서 시작
        // 아니면 0번 프레임에서 시작
        if (currentClip->bReversed)
            currentFrameIndex = currentClip->frameCount - 1;
        else
            currentFrameIndex = 0;

        // 현재 프레임 UV도 갱신
        currentFrame = currentClip->keyFrames[currentFrameIndex];
    }
}