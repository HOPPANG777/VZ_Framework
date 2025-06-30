#pragma once

#include "UnitBase.h"
#include "LumberAxe.h"
#include "Component/PlayerController.h"
#include "Component/InteractionComponent.h"

class PlayerUnit : public UnitBase
{
private:


public:
	PlayerUnit(const Vector3& position, const Vector3& scale = Vector3(100, 100, 0), const float rotation = 0.0f, const wstring& path = TexturePath + L"Animation/Player/Player_Idle_R.png");
	~PlayerUnit();

public:
	void Update();
	void Render();

};

