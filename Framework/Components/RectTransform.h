#pragma once

class RectTransform : public Component
{
private:
    Vector3 position = { 0, 0, 0 };
    Vector3 scale = { 1, 1, 1 };
    float   rotation = 0.0f;   // 라디안(radian)으로 보관

    Matrix  world = XMMatrixIdentity(); // S*R*T 결과

private:
    // 추가: UI 레이아웃을 위한 앵커와 오프셋
    Vector2 anchorMin = { 0.5f, 0.5f };   // 부모 기준 최소 앵커 (0~1)
    Vector2 anchorMax = { 0.5f, 0.5f };   // 부모 기준 최대 앵커 (0~1)
    Vector2 offsetMin = { -50.0f, -50.0f }; // 앵커에 상대적인 최소 오프셋 (픽셀 단위)
    Vector2 offsetMax = { 50.0f,  50.0f }; // 앵커에 상대적인 최대 오프셋 (픽셀 단위)

public:
    RectTransform(const string& compName = "RectTransform") : Component(compName) {}
    virtual ~RectTransform() override = default;

    virtual void Update() override;
    virtual void Render() override;

public:
    // Getter
    const Matrix& GetWorld() const;

    Vector3 GetPosition() const;
    Vector3 GetScale() const;
    float GetRotation() const; // 반환 시에는 Degree(각도)

    // 화면 크기를 기준으로 UI 요소의 RECT 반환
    RECT GetRect();

public:
    // Setter
    void SetPosition(const Vector3& pos);
    void SetScale(const Vector3& sca);
    void SetRotation(float degrees); // 외부 입력은 각도(°), 내부는 라디안

    // UI 레이아웃 설정: 앵커 및 오프셋 지정 (원하는 값으로 오버로드 가능)
    void SetAnchorAndOffset(const Vector2& newAnchorMin, const Vector2& newAnchorMax, const Vector2& newOffsetMin, const Vector2& newOffsetMax);

    // 기존 매개변수 없는 버전 (필요시 기본값 적용)
    void SetAnchorAndOffset();

public:
    // 이동/회전/스케일 변화를 DELTA 기반으로 적용할 수 있는 함수들
    void Move(const Vector3& speed);
    void Rotate(float degrees);
    void Scale(const Vector3& scale);

};