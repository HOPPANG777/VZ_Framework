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