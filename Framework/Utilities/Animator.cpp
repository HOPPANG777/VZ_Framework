#include "Framework.h"
#include "Animator.h"

AnimationClip::AnimationClip(
    const wstring& clipName,        // 애니메이션 클립의 이름
    shared_ptr<Texture2D> srcTex,   // 애니메이션이 저장된 텍스처
    UINT frameCount,                // 애니메이션의 프레임 개수
    Vector2 startPos,               // 텍스처에서 애니메이션이 시작되는 좌표
    Vector2 endPos,                 // 텍스처에서 애니메이션이 끝나는 좌표
    bool bReversed                  // 애니메이션의 역재생 여부
)
    : clipName(clipName), frameCount(frameCount), bReversed(bReversed)
{
    // 텍스처의 Shader Resource View(SRV)를 가져옵니다.
    srv = srcTex->GetSRV();

    // 텍스처의 너비와 높이를 가져옵니다.
    float imageWidth = srcTex->GetWidth();
    float imageHeight = srcTex->GetHeight();

    // 애니메이션 클립이 텍스처에서 차지하는 영역의 크기를 계산합니다.
    Vector2 clipSize = endPos - startPos;

    Vector2 frameSize;
    // 프레임 하나의 크기를 계산합니다.
    frameSize.x = clipSize.x / frameCount;  // 프레임 너비 = 클립 너비 / 프레임 개수
    frameSize.y = clipSize.y;              // 프레임 높이 = 클립 높이

    // 프레임의 텍셀 크기(UV 좌표 기준)를 계산합니다.
    texelFrameSize.x = frameSize.x / imageWidth;
    texelFrameSize.y = frameSize.y / imageHeight;

    // 애니메이션이 시작되는 텍스처의 시작 좌표를 텍셀 기준으로 변환합니다.
    Vector2 texelStartPos;
    texelStartPos.x = startPos.x / imageWidth;
    texelStartPos.y = startPos.y / imageHeight;

    // 각 프레임의 텍셀 좌표를 계산하여 keyFrames 벡터에 저장합니다.
    for (UINT i = 0; i < frameCount; ++i)
    {
        Vector2 keyFrame;
        keyFrame.x = texelStartPos.x + (texelFrameSize.x * i);  // 프레임의 x 좌표
        keyFrame.y = texelStartPos.y;                          // 프레임의 y 좌표

        // 프레임 좌표를 keyFrames 벡터에 추가합니다.
        keyFrames.push_back(keyFrame);
    }
}

void Animator::Update()
{
    // 현재 실행 중인 애니메이션 클립이 없는 경우 업데이트를 종료합니다.
    if (currentClip == nullptr) return;

    // 경과 시간이 현재 재생 속도를 초과했는지 확인합니다.
    if (deltaTime >= playRate)
    {
        if (!currentClip->IsReversed())  // 정방향 재생인 경우
        {
            // 현재 프레임 인덱스를 증가시킵니다.
            currentFrameIndex++;

            // 프레임 인덱스가 총 프레임 개수를 초과하면 처음으로 돌아갑니다.
            if (currentFrameIndex >= currentClip->GetFrameCount())
                currentFrameIndex = 0;

            // 현재 프레임 좌표를 갱신합니다.
            currentFrame = currentClip->GetKeyFrames()[currentFrameIndex];
        }
        else  // 역방향 재생인 경우
        {
            // 현재 프레임 인덱스를 감소시킵니다.
            if (currentFrameIndex == 0)
                currentFrameIndex = currentClip->GetFrameCount() - 1;  // 마지막 프레임으로 이동
            else
                currentFrameIndex--;

            // 현재 프레임 좌표를 갱신합니다.
            currentFrame = currentClip->GetKeyFrames()[currentFrameIndex];
        }

        // 경과 시간을 초기화합니다.
        deltaTime = 0.0f;
    }
    else
    {
        // 경과 시간을 증가시킵니다.
        deltaTime += TIME->GetDeltaTime();
    }
}


void Animator::AddAnimClip(shared_ptr<AnimationClip> animClip)
{
    // 애니메이션 클립을 이름을 키로 하여 unordered_map에 추가합니다.
    animClips[animClip->GetClipName()] = animClip;
}


void Animator::SetCurrentAnimClip(const wstring& clipName)
{
    // 이미 현재 실행 중인 클립과 동일한 클립을 설정하려고 하면 무시합니다.
    if (currentClip != nullptr && currentClip->GetClipName() == clipName)
        return;

    // 실행할 클립이 animClips에 존재하는지 확인합니다.
    if (CheckExist(clipName))
    {
        // 새로 설정할 클립을 현재 클립으로 설정합니다.
        currentClip = animClips[clipName];

        // 경과 시간을 초기화합니다.
        deltaTime = 0.0f;

        // 역방향 재생 여부에 따라 시작 프레임을 설정합니다.
        if (currentClip->IsReversed())
        {
            currentFrameIndex = currentClip->GetFrameCount() - 1;  // 마지막 프레임으로 설정
        }
        else
        {
            currentFrameIndex = 0;  // 첫 번째 프레임으로 설정
        }

        // 현재 프레임 좌표를 갱신합니다.
        currentFrame = currentClip->GetKeyFrames()[currentFrameIndex];
    }
}
