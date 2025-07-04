#pragma once

#include "Texts.h"

class ShakingText : public Texts
{
public:
    ShakingText(const std::string& name = "ShakingText", float intensity = 2.0f)
        : Texts(name), intensity(intensity) 
    {

    }

    void RenderText
    (
        const std::wstring& text,
        const RECT& rect,
        const float size = 25.0f,
        const Color& color = WHITE,
        const std::wstring& font = L"Pretendard JP",
        const DWRITE_FONT_WEIGHT& weight = DWRITE_FONT_WEIGHT_NORMAL,
        const DWRITE_FONT_STYLE& style = DWRITE_FONT_STYLE_NORMAL,
        const DWRITE_FONT_STRETCH& stretch = DWRITE_FONT_STRETCH_NORMAL
    ) override;


    void Update() override;
    void Render() override;

private:
    float intensity;

    int RandomOffset()
    {
        return int((rand() / float(RAND_MAX) - 0.5f) * 2 * intensity);
    }
};
