#include "stdafx.h"
#include "BezierCurve.h"

BezierCurveAxe::BezierCurveAxe(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path)
	: TextureRect(position, scale, rotation, path)
{

}

BezierCurveAxe::~BezierCurveAxe()
{

}

void BezierCurveAxe::Update()
{
	BASE::Update();
	
}

void BezierCurveAxe::Render()
{
	BASE::Render();
}
