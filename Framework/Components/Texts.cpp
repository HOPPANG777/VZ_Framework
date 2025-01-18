#include "Framework.h"
#include "Texts.h"

void Texts::RenderText(
    const wstring& text,
    const RECT& rect,
    const float size,
    const Color& color,
    const wstring& font,
    const DWRITE_FONT_WEIGHT& weight,
    const DWRITE_FONT_STYLE& style,
    const DWRITE_FONT_STRETCH& stretch)
{
    FontBrushDesc* brush = GetOrCreateBrush(color);
    FontTextDesc* textDesc = GetOrCreateTextFormat(font, size, weight, style, stretch);

    RECT finalRect = CalculateRectFromObject(rect);

    D2D1_RECT_F rectF = {
        static_cast<float>(finalRect.left),
        static_cast<float>(finalRect.top),
        static_cast<float>(finalRect.right),
        static_cast<float>(finalRect.bottom)
    };

    D2DC->DrawTextW(
        text.c_str(),
        static_cast<UINT32>(text.size()),
        textDesc->format.Get(),
        rectF,
        brush->brush.Get()
    );
}

void Texts::AttachToObject(Object* object, const Vector3& offset)
{
    this->attachedObject = object;
    this->offset = offset;
}

void Texts::Detach()
{
    this->attachedObject = nullptr;
}

FontBrushDesc* Texts::GetOrCreateBrush(const Color& color)
{
    auto newBrushDesc = make_unique<FontBrushDesc>();
    newBrushDesc->color = color;

    auto brushIt = find_if(fontBrush.begin(), fontBrush.end(),
        [&newBrushDesc](const unique_ptr<FontBrushDesc>& ptr) {
            return *ptr == *newBrushDesc;
        });

    if (brushIt != fontBrush.end())
        return brushIt->get();

    D2D1::ColorF colorF(color.R(), color.G(), color.B(), color.A());
    HRESULT hr = D2DC->CreateSolidColorBrush(colorF, &newBrushDesc->brush);
    CHECK(hr);

    FontBrushDesc* createdBrush = newBrushDesc.get();
    fontBrush.push_back(move(newBrushDesc));
    return createdBrush;
}

FontTextDesc* Texts::GetOrCreateTextFormat(
    const wstring& font,
    float size,
    const DWRITE_FONT_WEIGHT& weight,
    const DWRITE_FONT_STYLE& style,
    const DWRITE_FONT_STRETCH& stretch)
{
    auto newTextDesc = make_unique<FontTextDesc>();
    newTextDesc->font = font;
    newTextDesc->fontSize = size;
    newTextDesc->weight = weight;
    newTextDesc->style = style;
    newTextDesc->stretch = stretch;

    auto textIt = find_if(fontText.begin(), fontText.end(),
        [&newTextDesc](const unique_ptr<FontTextDesc>& ptr) {
            return *ptr == *newTextDesc;
        });

    if (textIt != fontText.end())
        return textIt->get();

    HRESULT hr = WRITE->CreateTextFormat(
        newTextDesc->font.c_str(),
        nullptr,
        newTextDesc->weight,
        newTextDesc->style,
        newTextDesc->stretch,
        newTextDesc->fontSize,
        L"ko",
        &newTextDesc->format
    );
    CHECK(hr);

    FontTextDesc* createdTextDesc = newTextDesc.get();
    fontText.push_back(move(newTextDesc));
    return createdTextDesc;
}

RECT Texts::CalculateRectFromObject(const RECT& rect)
{
    if (!attachedObject)
        return rect;

    Vector3 objectPos = attachedObject->GetPosition(); // °´Ã¼ÀÇ ¿ùµå ÁÂÇ¥
    RECT adjustedRect = rect;

    adjustedRect.left += LONG(objectPos.x + offset.x);
    adjustedRect.right += LONG(objectPos.x + offset.x);
    adjustedRect.top += LONG(objectPos.y + offset.y);
    adjustedRect.bottom += LONG(objectPos.y + offset.y);

    return adjustedRect;
}
