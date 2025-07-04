#pragma once

#include "HitComponent.h"
#include "Components/Component.h"
#include "Components/BoxCollider.h"

class LadderTransComponent : public Component
{
public:
	LadderTransComponent(const std::string& name = "LadderTransComponent") : Component(name), collisionCount(0), transformed(false), newColliderSize(100, 520, 0) {}
	~LadderTransComponent() override = default;

public:
	void Update() override {}
	void Render() override {}

public:
	void OnAxeCollision();
	void OnLadderCollision(Collider* other);

public:
	bool GetTransformed() const { return transformed; }

private:
	bool transformed = false;
	int collisionCount = 0;
	Vector3 newColliderSize = Vector3(100, 500, 0);

};

