#pragma once

#include "UnitBase.h"

class EnemyUnit : public UnitBase
{
private:


public:
	EnemyUnit(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path);
	~EnemyUnit();

public:
	void Update();
	void Render();

};

