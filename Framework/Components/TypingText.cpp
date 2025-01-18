#include "Framework.h"
#include "TypingText.h"

void TypingText::RenderText(
    const wstring& text, 
    const RECT& rect, 
    const float size, 
    const Color& color, 
    const wstring& font, 
    const DWRITE_FONT_WEIGHT& weight, 
    const DWRITE_FONT_STYLE& style, 
    const DWRITE_FONT_STRETCH& stretch)
{
    wstring visibleText = text.substr(0, visibleCharacters);
    Texts::RenderText(visibleText, rect, size, color, font, weight, style, stretch);
}

void TypingText::Update()
{
    elapsedTime += DELTA;
    if (elapsedTime >= typingSpeed)
    {
        elapsedTime = 0.0f;
        visibleCharacters++;
    }
}

void TypingText::Render()
{

}
