#pragma once

#include "UnitBase.h"
#include "LumberAxe.h"

class PlayerUnit : public UnitBase
{
private:


public:
	PlayerUnit(const Vector3& position, const Vector3& scale = Vector3(100, 100, 0), const float rotation = 0.0f, const wstring& path = L"");
	~PlayerUnit();

public:
	void Update();
	void Render();

};

