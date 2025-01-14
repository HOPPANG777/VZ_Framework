#pragma once

struct FontBrushDesc
{
	Color color;
	ComPtr<ID2D1SolidColorBrush> brush = nullptr;

	bool operator==(const FontBrushDesc& val) const
	{
		return	color.R() == val.color.R() &&
				color.G() == val.color.G() &&
				color.B() == val.color.B() &&
				color.A() == val.color.A();
	}
};

struct FontTextDesc
{
	wstring font;
	float fontSize;

	DWRITE_FONT_WEIGHT weight;
	DWRITE_FONT_STYLE style;
	DWRITE_FONT_STRETCH stretch;

	ComPtr<IDWriteTextFormat> format = nullptr;

	bool operator==(const FontTextDesc& val)
	{
		bool b = true;
		b &= font == val.font;
		b &= fontSize == val.fontSize;
		b &= weight == val.weight;
		b &= style == val.style;
		b &= stretch == val.stretch;

		return b;
	}
};


class Fonts
{
	DECLARE_SINGLETON(Fonts);

public:
	void GUI();

public:
	void RenderText
	(
		  const wstring& text
		, const RECT& rect
		, const float size = 20.0f
		, const wstring& font = L"Pretendard-Regular"//L"en-us",//L"ko-kr",
		, const Color& color = WHITE
		, const DWRITE_FONT_WEIGHT& weight = DWRITE_FONT_WEIGHT_NORMAL
		, const DWRITE_FONT_STYLE& style = DWRITE_FONT_STYLE_NORMAL
		, const DWRITE_FONT_STRETCH& stretch = DWRITE_FONT_STRETCH_NORMAL
	);


private:
	static vector<unique_ptr<FontBrushDesc>> fontBrush;
	static vector<unique_ptr<FontTextDesc>> fontText;

};

