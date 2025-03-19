#pragma once

#include "Objects/TextureObject/TextureRect.h"
#include "Component/BezierCurveAxe.h"

class LumberAxe : public TextureRect
{
public:
	LumberAxe(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path = TexturePath + L"NordicLumberjack/Weapon/Axe.png");
	~LumberAxe();

public:
	void Update();
	void Render();

};