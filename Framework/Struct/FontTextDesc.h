#pragma once

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
