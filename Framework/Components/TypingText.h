#pragma once

class TypingText : public Texts
{
public:
    TypingText(const string& name = "TypingText", float typingSpeed = 0.1f)
        : Texts(name), typingSpeed(typingSpeed), elapsedTime(0.0f), visibleCharacters(0) {
    }

    void RenderText(
        const wstring& text,
        const RECT& rect,
        const float size = 25.0f,
        const Color& color = WHITE,
        const wstring& font = L"Pretendard JP",
        const DWRITE_FONT_WEIGHT& weight = DWRITE_FONT_WEIGHT_NORMAL,
        const DWRITE_FONT_STYLE& style = DWRITE_FONT_STYLE_NORMAL,
        const DWRITE_FONT_STRETCH& stretch = DWRITE_FONT_STRETCH_NORMAL
    ) override;

    void Update() override;
    void Render() override;

private:
    float typingSpeed; // 한 글자당 표시 속도
    float elapsedTime;
    size_t visibleCharacters;
};