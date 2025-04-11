#pragma once

class LadderComponent : public Component
{
public:
	LadderComponent(const string& name = "LadderComponent") : Component(name) {}
	virtual ~LadderComponent() override = default;

public:
	virtual void Update() override;
	virtual void Render() override;

private:

};

