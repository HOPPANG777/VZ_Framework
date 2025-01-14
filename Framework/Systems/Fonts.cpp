#include "Framework.h"
#include "Fonts.h"


// static 멤버 초기화
vector<unique_ptr<FontBrushDesc>> Fonts::fontBrush;
vector<unique_ptr<FontTextDesc>> Fonts::fontText;

Fonts::Fonts(){}

Fonts::~Fonts() {}

void Fonts::GUI()
{
    // 필요한 GUI 코드 구현
}

void Fonts::RenderText(
    const wstring& text,
    const RECT& rect,
    const float size,
    const wstring& font,
    const Color& color,
    const DWRITE_FONT_WEIGHT& weight,
    const DWRITE_FONT_STYLE& style,
    const DWRITE_FONT_STRETCH& stretch)
{
    // Graphics 인스턴스를 통해 Direct2D 컨텍스트 가져오기
    //Graphics& graphics = GRAPHICS;

    // 새로운 FontBrushDesc 객체 생성 및 초기화
    auto newBrushDesc = make_unique<FontBrushDesc>();
    newBrushDesc->color = color;

    // fontBrush 컨테이너에서 같은 색상의 브러시 찾기
    FontBrushDesc* findBrush = nullptr;
    auto brushIt = find_if(fontBrush.begin(), fontBrush.end(),
        [&newBrushDesc](const unique_ptr<FontBrushDesc>& ptr) {
            return *ptr == *newBrushDesc;
        });

    if (brushIt != fontBrush.end())
    {
        findBrush = brushIt->get();
    }
    else
    {
        D2D1::ColorF colorF = D2D1::ColorF(color.R(), color.G(), color.B(), color.A());
        HRESULT hr = D2DC->CreateSolidColorBrush(colorF, &newBrushDesc->brush);
        CHECK(hr);

        findBrush = newBrushDesc.get();
        fontBrush.push_back(std::move(newBrushDesc));
    }

    // FontTextDesc 처리
    auto newTextDesc = make_unique<FontTextDesc>();
    newTextDesc->font = font;
    newTextDesc->fontSize = size;
    newTextDesc->stretch = stretch;
    newTextDesc->style = style;
    newTextDesc->weight = weight;

    FontTextDesc* findText = nullptr;
    auto textIt = find_if(fontText.begin(), fontText.end(),
        [&newTextDesc](const unique_ptr<FontTextDesc>& ptr) {
            return *ptr == *newTextDesc;
        });

    if (textIt != fontText.end())
    {
        findText = textIt->get();
    }
    else
    {
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

        findText = newTextDesc.get();
        fontText.push_back(std::move(newTextDesc));
    }

    // 텍스트 그리기
    D2D1_RECT_F rectF;
    rectF.left = static_cast<float>(rect.left);
    rectF.right = static_cast<float>(rect.right);
    rectF.top = static_cast<float>(rect.top);
    rectF.bottom = static_cast<float>(rect.bottom);

    // 텍스트를 그리려면 DirectWrite의 DrawTextW 대신 Direct2D의 DrawText 등을 사용해야 함
    // ID2D1DeviceContext::DrawTextW를 사용하여 텍스트를 그립니다.
    D2DC->DrawTextW(
        text.c_str(),
        static_cast<UINT32>(text.size()),
        findText->format.Get(),
        rectF,
        findBrush->brush.Get()
    );
}
