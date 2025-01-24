#pragma once

class Texts : public Component
{
public:
    Texts(const string& name = "Texts") : Component(name) {}
    virtual ~Texts() = default;

    virtual void RenderText(
        const wstring& text,
        const RECT& rect,
        const float size = 25.0f,
        const Color& color = WHITE,
        const wstring& font = L"Pretendard JP",
        const DWRITE_FONT_WEIGHT& weight = DWRITE_FONT_WEIGHT_NORMAL,
        const DWRITE_FONT_STYLE& style = DWRITE_FONT_STYLE_NORMAL,
        const DWRITE_FONT_STRETCH& stretch = DWRITE_FONT_STRETCH_NORMAL
    );

    virtual void Update() override {} // 애니메이션을 구현하려면 업데이트 메서드 사용
    virtual void Render() override {}

public:
    void AttachToObject(Object* object, const Vector3& offset = Values::ZeroVec3);
    void Detach();

protected:
    vector<unique_ptr<FontBrushDesc>> fontBrush;
    vector<unique_ptr<FontTextDesc>> fontText;

    FontBrushDesc* GetOrCreateBrush(const Color& color);
    FontTextDesc* GetOrCreateTextFormat(
        const wstring& font,
        float size,
        const DWRITE_FONT_WEIGHT& weight,
        const DWRITE_FONT_STYLE& style,
        const DWRITE_FONT_STRETCH& stretch
    );

    Object* attachedObject = nullptr; // 텍스트가 따라다닐 객체
    Vector3 offset = Values::ZeroVec3;  // 객체 위치에 더할 오프셋

private:
    RECT CalculateRectFromObject(const RECT& rect);
};