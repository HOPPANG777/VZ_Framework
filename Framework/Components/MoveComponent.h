#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	MoveComponent(const string& name = "Move", const float maxSpeed = 300.0f) : Component(name), maxSpeed(maxSpeed) {}
	~MoveComponent() override = default;

public:
	// ComponentÀ»(¸¦) ÅëÇØ »ó¼ÓµÊ
	void Update() override;
	void Render() override;

	void AddKeyCount() { ++keyCount; }
	void ResetKeyCount() { keyCount = 0; }

private:
	float maxSpeed = 300.0f;
	float speed = maxSpeed;
	UINT keyCount = 0;

};

