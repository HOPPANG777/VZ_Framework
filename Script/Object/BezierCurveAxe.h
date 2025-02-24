#pragma once

#include "Objects/TextureObject/TextureRect.h"

class BezierCurveAxe : public TextureRect
{
private:

public:
	BezierCurveAxe(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path = TexturePath + L"NordicLumberjack/Weapon/Axe.png");
	~BezierCurveAxe();

public:
	void Update();
	void Render();

public:


};