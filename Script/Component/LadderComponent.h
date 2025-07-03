#pragma once

#include "PlayerController.h"
#include "Components/Component.h"
#include "Components/BoxCollider.h"
#include "Components/Transform.h"
#include "Managers/SceneManager.h"

class LadderComponent : public Component
{
public:
	LadderComponent(const string& name = "LadderComponent") : Component(name) {}
	virtual ~LadderComponent() override = default;

public:
	virtual void Update() override;
	virtual void Render() override {}

};

