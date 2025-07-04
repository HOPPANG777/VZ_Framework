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
    // 부모 사각형 (여기서는 전체 창 크기를 사용)
    RECT parentRect = { -(LONG)(Values::GetWinWidth() * 0.5f), -(LONG)(Values::GetWinHeight() * 0.5f), (LONG)(Values::GetWinWidth() * 0.5f), (LONG)(Values::GetWinHeight() * 0.5f) };
    LONG parentWidth  = (parentRect.right  - parentRect.left);
    LONG parentHeight = (parentRect.bottom - parentRect.top);


    // 앵커와 오프셋을 이용하여 기본 사각형 계산
    float anchorPosMinX = parentRect.left + anchorMin.x * parentWidth  + offsetMin.x;
    float anchorPosMinY = parentRect.top  + anchorMin.y * parentHeight + offsetMin.y;
    float anchorPosMaxX = parentRect.left + anchorMax.x * parentWidth  + offsetMax.x;
    float anchorPosMaxY = parentRect.top  + anchorMax.y * parentHeight + offsetMax.y;

    // 기본 사각형의 중심 및 크기
    float anchoredCenterX = (anchorPosMinX + anchorPosMaxX) * 0.5f;
    float anchoredCenterY = (anchorPosMinY + anchorPosMaxY) * 0.5f;
    float anchoredWidth   = anchorPosMaxX - anchorPosMinX;
    float anchoredHeight  = anchorPosMaxY - anchorPosMinY;

    // 추가로 position 값을 더해 최종 중심 계산 (앵커 기준 오프셋)
    float finalCenterX = anchoredCenterX + position.x;
    float finalCenterY = anchoredCenterY + position.y;

    // 추가 scale을 기본 크기에 반영 (너비, 높이 변화량)
    float finalWidth  = anchoredWidth + scale.x;
    float finalHeight = anchoredHeight + scale.y;

    // 최종 중심과 크기를 이용해 RECT 계산 (중심 기준 좌표)
    LONG left   = (LONG)(finalCenterX - finalWidth * 0.5f);
    LONG right  = (LONG)(finalCenterX + finalWidth * 0.5f);
    LONG top    = (LONG)(finalCenterY - finalHeight * 0.5f);
    LONG bottom = (LONG)(finalCenterY + finalHeight * 0.5f);

    RECT rect = { left, top, right, bottom };
    return rect;
}

RECT RectTransform::GetCorrectedRect()
{
    // 1) (0,0)이 좌측 상단, Y 아래로 증가하는 GetRect()
    RECT rect = GetRect();

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
    position += speed * Time::Get()->GetDeltaTime();
}

void RectTransform::Rotate(float degrees)
{
    rotation += XMConvertToRadians(degrees) * Time::Get()->GetDeltaTime();
}

void RectTransform::Scale(const Vector3& scale)
{
    this->scale += scale * Time::Get()->GetDeltaTime();
}