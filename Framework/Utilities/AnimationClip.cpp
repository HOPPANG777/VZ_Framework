#include "Framework.h"
#include "AnimationClip.h"

AnimationClip::AnimationClip(const std::wstring& clipName,
    Texture2D* srcTex,
    UINT columns,    // 가로 프레임 수
    UINT rows,       // 세로 프레임 수
    Vector2 startPos,
    Vector2 endPos,
    bool bReversed
    //float playRate
    )
    : clipName(clipName),
    // 전체 프레임 수 = columns * rows
    frameCount(columns * rows),
    bReversed(bReversed)//, playRate(playRate) // ★ 멤버에 저장
{
    srv = srcTex->GetSRV();

    const float imageWidth = srcTex->GetWidth();
    const float imageHeight = srcTex->GetHeight();

    // 지정된 영역
    const Vector2 clipSize = endPos - startPos;

    // 프레임 하나의 크기(픽셀 단위)
    const float frameWidth = clipSize.x / (float)columns;
    const float frameHeight = clipSize.y / (float)rows;

    // UV 변환
    texelFrameSize.x = frameWidth / imageWidth;
    texelFrameSize.y = frameHeight / imageHeight;

    // 시작점 UV
    Vector2 texelStartPos;
    texelStartPos.x = startPos.x / imageWidth;
    texelStartPos.y = startPos.y / imageHeight;

    // 전체 프레임 = columns * rows
    keyFrames.reserve(frameCount);

    // 행(0 ~ rows-1), 열(0 ~ columns-1) 순회
    for (UINT row = 0; row < rows; row++)
    {
        for (UINT col = 0; col < columns; col++)
        {
            Vector2 keyFrame;
            keyFrame.x = texelStartPos.x + texelFrameSize.x * col;
            keyFrame.y = texelStartPos.y + texelFrameSize.y * row;

            keyFrames.push_back(keyFrame);
        }
    }
}