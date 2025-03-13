#pragma once

#include "UnitBase.h"

class PlayerUnit : public UnitBase
{
private:


public:
	PlayerUnit(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path);
	~PlayerUnit();

public:
	void Update();
	void Render();

};

