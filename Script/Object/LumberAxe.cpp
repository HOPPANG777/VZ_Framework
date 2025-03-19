#include "stdafx.h"
#include "LumberAxe.h"

LumberAxe::LumberAxe(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path)
	: TextureRect(position, scale, rotation, path)
{

	AddComponent(make_shared<BezierCurveAxe>());

}

LumberAxe::~LumberAxe()
{

}

void LumberAxe::Update()
{
	BASE::Update();
}

void LumberAxe::Render()
{
	BASE::Render();
}
