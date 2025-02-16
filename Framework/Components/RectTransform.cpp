#include "Framework.h"
#include "RectTransform.h"

void RectTransform::Update()
{
    // S, R, T 행렬 계산
    Matrix S = XMMatrixScalingFromVector(scale);
    Matrix R = XMMatrixRotationZ(rotation);
    Matrix T = XMMatrixTranslationFromVector(position);

    world = S * R * T;
}

void RectTransform::Render()
{
    // RectTransform은 시각적으로 그릴 요소가 없으므로 보통 비웁니다.
    // (필요 시, 디버그용 기즈모 등으로 확장 가능)
}

Vector3 RectTransform::GetPosition() const
{
    return position;
}

Vector3 RectTransform::GetScale() const
{
    return scale;
}

float RectTransform::GetRotation() const
{
    // 내부는 라디안이지만, 외부엔 각도(도)로 반환
    return XMConvertToDegrees(rotation);
}

// 부모의 RECT 정보가 없다면, 전역 변수로 관리되는 창 크기를 사용합니다.
RECT RectTransform::GetRect()
{
    // 부모 사각형 (실제 UI 시스템에서는 부모의 RectTransform에서 얻어야 합니다)
    RECT parentRect = { 0, 0, (long)gWinWidth, (long)gWinHeight };

    float parentWidth = (float)(parentRect.right - parentRect.left);
    float parentHeight = (float)(parentRect.bottom - parentRect.top);

    // 앵커에 따른 기준 좌표 계산
    float anchorPosMinX = parentRect.left + anchorMin.x * parentWidth;
    float anchorPosMinY = parentRect.top + anchorMin.y * parentHeight;
    float anchorPosMaxX = parentRect.left + anchorMax.x * parentWidth;
    float anchorPosMaxY = parentRect.top + anchorMax.y * parentHeight;

    // 오프셋 적용: 오프셋은 픽셀 단위로 좌표에 더해집니다.
    long left = (long)(anchorPosMinX + offsetMin.x);
    long top = (long)(anchorPosMinY + offsetMin.y);
    long right = (long)(anchorPosMaxX + offsetMax.x);
    long bottom = (long)(anchorPosMaxY + offsetMax.y);

    RECT rect = { left, top, right, bottom };
    return rect;
}

const Matrix& RectTransform::GetWorld() const
{
    return world;
}

void RectTransform::SetPosition(const Vector3& pos)
{
    position = pos;
}

void RectTransform::SetScale(const Vector3& sca)
{
    scale = sca;
}

void RectTransform::SetRotation(float degrees)
{
    rotation = XMConvertToRadians(degrees);
}

void RectTransform::SetAnchorAndOffset(const Vector2& newAnchorMin, const Vector2& newAnchorMax, const Vector2& newOffsetMin, const Vector2& newOffsetMax)
{
    anchorMin = newAnchorMin;
    anchorMax = newAnchorMax;
    offsetMin = newOffsetMin;
    offsetMax = newOffsetMax;
}

// 기본 버전: 기본 앵커/오프셋 값 적용 (이미 초기값이 있으므로 그대로 둠)
void RectTransform::SetAnchorAndOffset()
{
    // 여기에서 추가적인 로직을 넣을 수도 있습니다.
}

void RectTransform::Move(const Vector3& speed)
{
    position += speed * DELTA;
}

void RectTransform::Rotate(float degrees)
{
    rotation += XMConvertToRadians(degrees) * DELTA;
}

void RectTransform::Scale(const Vector3& scale)
{
    this->scale += scale * DELTA;
}
