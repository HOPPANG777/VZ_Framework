#pragma once

#include "Component.h"
#include "Systems/Time.h"

class Transform : public Component
{
private:
    Vector3 position = { 0, 0, 0 };
    Vector3 scale = { 1, 1, 1 };
    float   rotation = 0.0f;   // 라디안(radian)으로 보관

    Matrix  world = XMMatrixIdentity(); // S*R*T 결과

public:
    // 기본적으로 "Transform"이라는 이름으로 생성
    Transform(const std::string& compName = "Transform") : Component(compName) {}
    virtual ~Transform() override = default;

    virtual void Update() override;  // 매 프레임 S*R*T 계산
    virtual void Render() override;  // 특별히 그릴 것은 없으므로 보통 빈 상태

    const Matrix& GetWorld() const;

public:
    // Getter
    Vector3 GetPosition() const;
    Vector3 GetScale() const;
    float GetRotation() const; // 반환 시에는 Degree(각도)

public:
    // Setter
    void SetPosition(const Vector3& pos);
    void SetScale(const Vector3& sca);
    void SetRotation(float degrees); // 외부 입력은 각도(°), 내부는 라디안

public:
    // 이동/회전/스케일 변화를 DELTA 기반으로 적용할 수 있는 함수들
    void Move(const Vector3& speed);
    void Rotate(float degrees);
    void Scale(const Vector3& scale);

};