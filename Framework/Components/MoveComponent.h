#pragma once

class MoveComponent : public Component
{
public:
	MoveComponent(const float maxSpeed = 300.0f, const string& name = "Move") : Component(name), maxSpeed(maxSpeed) {}
	~MoveComponent() override = default;

public:
	// Component을(를) 통해 상속됨
	void Update() override;
	void Render() override;

	void AddKeyCount() { ++keyCount; }
	void ResetKeyCount() { keyCount = 0; }

private:
	float maxSpeed = 300.0f;
	float speed = maxSpeed;
	UINT keyCount = 0;

};

