#include "stdafx.h"
#include "EnemyUnit.h"

EnemyUnit::EnemyUnit(const Vector3& position, const Vector3& scale, const float rotation, const std::wstring& path)
	: UnitBase(position, scale, rotation, path)
{

}

EnemyUnit::~EnemyUnit()
{
}

void EnemyUnit::Update()
{
	BASE::Update();
}

void EnemyUnit::Render()
{
	BASE::Render();
}
