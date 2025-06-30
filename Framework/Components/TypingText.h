#pragma once

class TypingText : public Texts
{
public:
    TypingText(const string& name = "TypingText", float typingSpeed = 0.075f)
        : Texts(name), typingSpeed(typingSpeed), elapsedTime(0.0f), visibleCharacters(0)
    {

    }

    // RenderText: 타이핑 효과로 출력된 텍스트를 렌더링 (기존 Texts::RenderText를 내부적으로 호출)
    virtual void RenderText
    (
        const wstring& text,
        const RECT& rect,
        const float size = 25.0f,
        const Color& color = WHITE,
        const wstring& font = L"Pretendard JP",
        const DWRITE_FONT_WEIGHT& weight = DWRITE_FONT_WEIGHT_NORMAL,
        const DWRITE_FONT_STYLE& style = DWRITE_FONT_STYLE_NORMAL,
        const DWRITE_FONT_STRETCH& stretch = DWRITE_FONT_STRETCH_NORMAL
    ) override;

public:
    virtual void Update() override;
    virtual void Render() override;

public:
    // 텍스트 출력이 모두 완료되었는지 여부 반환
    bool IsComplete() const { return complete; }
    // 텍스트 출력 스킵
	void SkipToEnd() { visibleCharacters = fullText.size(); complete = true; }

private:
    float typingSpeed;        // 한 글자 출력에 걸리는 시간 (초)
    float elapsedTime;        // 누적 시간
    size_t visibleCharacters; // 현재까지 출력된 문자 수
    wstring fullText;         // 전체 텍스트 내용
    bool complete = false;    // 텍스트 출력 완료 여부

};