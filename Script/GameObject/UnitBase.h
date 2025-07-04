#pragma once

#include "Objects/TextureObject/AnimationRect.h"

class UnitBase : public AnimationRect
{
private:


public:
	UnitBase(const Vector3& position, const Vector3& scale, const float rotation, const std::wstring& path);
	~UnitBase();

public:
	void Update();
	void Render();

};

