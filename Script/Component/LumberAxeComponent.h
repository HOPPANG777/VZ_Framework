#pragma once

class LumberAxeComponent : public Component
{
public:
	LumberAxeComponent(const string& name = "LumberAxeComponent") : Component(name) {}
	~LumberAxeComponent() override = default;

public:
	// Component을(를) 통해 상속됨
	void Update() override;
	void Render() override;

};

