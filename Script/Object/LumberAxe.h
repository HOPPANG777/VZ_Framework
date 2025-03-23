#pragma once

//#include "Objects/TextureObject/TextureRect.h"
#include "Component/LumberAxeComponent.h"

class LumberAxe : public TextureRect
{
public:
	LumberAxe(const Vector3& position, const Vector3& scale = Vector3(50, 50, 0), const float rotation = 0.0f, const wstring& path = TexturePath + L"Weapon/Axe.png");
	~LumberAxe();

public:
	void Update();
	void Render();

public:


private:
	shared_ptr<LumberAxeComponent> lumberAxeComponent;

};