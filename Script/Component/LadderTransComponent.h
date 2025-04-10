#pragma once

#include "HitComponent.h"

class LadderTransComponent : public Component
{
public:
	LadderTransComponent(const std::string& name = "LadderTransComponent") : Component(name), collisionCount(0), transformed(false), newColliderSize(400, 100, 0) {}
	~LadderTransComponent() override = default;

public:
	void Update() override {}
	void Render() override {}

public:
	void OnAxeCollision();
	void OnLadderCollision(Collider* other);

private:
	bool transformed = false;
	int collisionCount = 0;
	Vector3 newColliderSize = Vector3(100, 500, 0);

};

