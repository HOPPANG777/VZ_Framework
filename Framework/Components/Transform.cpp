#include "Framework.h"
#include "Transform.h"

Transform::Transform(const std::string& compName)
    : Component(compName)
{

}

void Transform::Update()
{
    // S, R, T 행렬 계산
    Matrix S = XMMatrixScalingFromVector(scale);
    Matrix R = XMMatrixRotationZ(rotation);
    Matrix T = XMMatrixTranslationFromVector(position);

    world = S * R * T;
}

void Transform::Render()
{
    // Transform은 시각적으로 그릴 요소가 없으므로 보통 비웁니다.
    // (필요 시, 디버그용 기즈모 등으로 확장 가능)
}

Vector3 Transform::GetPosition() const
{
    return position;
}

Vector3 Transform::GetScale() const
{
    return scale;
}

float Transform::GetRotation() const
{
    // 내부는 라디안이지만, 외부엔 각도(도)로 반환
    return XMConvertToDegrees(rotation);
}

const Matrix& Transform::GetWorld() const
{
    return world;
}

void Transform::SetPosition(const Vector3& pos)
{
    position = pos;
}

void Transform::SetScale(const Vector3& sca)
{
    scale = sca;
}

void Transform::SetRotation(float degrees)
{
    rotation = XMConvertToRadians(degrees);
}

void Transform::Move(const Vector3& speed)
{
    position += speed * DELTA;
}

void Transform::Rotate(float degrees)
{
    rotation += XMConvertToRadians(degrees) * DELTA;
}

void Transform::Scale(const Vector3& scale)
{
    this->scale += scale * DELTA;
}