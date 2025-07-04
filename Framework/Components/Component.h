#pragma once

class GameObject;
class Component
{
protected:
	std::string name;
	GameObject* owner = nullptr;

public:
	Component(const std::string& compName) : name(compName) {}
	virtual ~Component() = default;

public:
	GameObject* GetOwner() const { return owner; }
	std::string GetName() const { return name; }

	void SetOwner(GameObject* obj) { owner = obj; }

public:
	virtual void Update() = 0;
	virtual void Render() = 0;

};