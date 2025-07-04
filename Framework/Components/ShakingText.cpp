#include "ShakingText.h"
void ShakingText::RenderText(
    const std::wstring& text, 
    const RECT& rect, 
    const float size, 
    const Color& color, 
    const std::wstring& font, 
    const DWRITE_FONT_WEIGHT& weight, 
    const DWRITE_FONT_STYLE& style, 
    const DWRITE_FONT_STRETCH& stretch)
{
    RECT newRect = rect;
    newRect.left += RandomOffset();
    newRect.top += RandomOffset();

    Texts::RenderText(text, newRect, size, color, font, weight, style, stretch);
}

void ShakingText::Update()
{

}

void ShakingText::Render()
{

}
