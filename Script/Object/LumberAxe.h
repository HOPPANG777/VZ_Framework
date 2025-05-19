#pragma once

//#include "Objects/TextureObject/TextureRect.h"
#include "Component/LumberAxeComponent.h"
#include "Component/LumberAxeAnimation.h"

class LumberAxe : public AnimationRect
{
public:
	LumberAxe(const Vector3& position, const Vector3& scale = Vector3(100, 100, 0), const float rotation = 0.0f, const wstring& path = TexturePath + L"Weapon/Axe_R.png");
	~LumberAxe();

public:
	void Update();
	void Render();

public:


private:
	shared_ptr<LumberAxeComponent> lumberAxeComponent;

};