#include "Framework.h"
#include "TypingText.h"

void TypingText::RenderText
(
	const wstring& text,
	const RECT& rect,
	const float size,
	const Color& color,
	const wstring& font,
	const DWRITE_FONT_WEIGHT& weight,
	const DWRITE_FONT_STYLE& style,
	const DWRITE_FONT_STRETCH& stretch
)
{
	// 전체 텍스트(fullText) 대신, visibleCharacters만큼 자른 텍스트를 렌더링
	wstring visibleText = text.substr(0, visibleCharacters);
	fullText = text;
	Texts::RenderText(visibleText, rect, size, color, font, weight, style, stretch);
}

void TypingText::Update()
{
	// 여기서 기본 Texts::Update()가 특별한 로직이 없다면 생략하고 타이핑 업데이트만 수행
	elapsedTime += DELTA;
	if (!complete && elapsedTime >= typingSpeed)
	{
		// typingSpeed마다 한 글자씩 노출
		visibleCharacters++;
		elapsedTime = 0.0f;
		if (visibleCharacters >= fullText.size())
		{
			visibleCharacters = fullText.size();
			complete = true;
		}
	}
}

void TypingText::Render()
{

}
