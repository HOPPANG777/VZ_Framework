#include "Framework.h"
#include "Gizmo.h"

Gizmo::Gizmo() {}

Gizmo::~Gizmo() {}

void Gizmo::DrawBox(const Vector3& center, const Vector3& size, const Color& color, float thickness)
{
	if (!Graphics::Get()->GetD2DC()) return;

	D2D1_RECT_F rect = { center.x - size.x / 2,gWinHeight - (center.y + size.y / 2),center.x + size.x / 2,gWinHeight - (center.y - size.y / 2) };

	ComPtr<ID2D1SolidColorBrush> brush;
	Graphics::Get()->GetD2DC()->CreateSolidColorBrush(ColorF(color.R(), color.G(), color.B(), color.A()), &brush);

	Graphics::Get()->GetD2DC()->DrawRectangle(&rect, brush.Get(), thickness);
}

void Gizmo::DrawCircle(const Vector3& center, float radius, const Color& color, float thickness)
{
	if (!Graphics::Get()->GetD2DC()) return;

	ComPtr<ID2D1SolidColorBrush> brush;
	Graphics::Get()->GetD2DC()->CreateSolidColorBrush(ColorF(color.R(), color.G(), color.B(), color.A()), &brush);

	D2D1_ELLIPSE ellipse = { { center.x, gWinHeight - center.y }, radius, radius };
	Graphics::Get()->GetD2DC()->DrawEllipse(&ellipse, brush.Get(), thickness);
}

void Gizmo::DrawLine(const Vector3& start, const Vector3& end, const Color& color, float thickness)
{
	if (!Graphics::Get()->GetD2DC()) return;

	ComPtr<ID2D1SolidColorBrush> brush;
	Graphics::Get()->GetD2DC()->CreateSolidColorBrush(ColorF(color.R(), color.G(), color.B(), color.A()), &brush);

	Graphics::Get()->GetD2DC()->DrawLine(Point2F(start.x, gWinHeight - start.y), Point2F(end.x, gWinHeight - end.y), brush.Get(), thickness);
}


