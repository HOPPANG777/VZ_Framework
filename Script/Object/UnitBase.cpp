#include "stdafx.h"
#include "UnitBase.h"

UnitBase::UnitBase(const Vector3& position, const Vector3& scale, const float rotation, const wstring& path)
    : AnimationRect(position, scale, rotation, path)
{
    // 플레이어는 조작 가능하도록 MoveComponent를 추가합니다.
    
}

UnitBase::~UnitBase()
{

}

void UnitBase::Update()
{
	BASE::Update();
}

void UnitBase::Render()
{
	BASE::Render();
}
