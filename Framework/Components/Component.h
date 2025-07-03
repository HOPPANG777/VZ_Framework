#pragma once

class Object;
class Component
{
protected:
	std::string name;
	Object* owner = nullptr;

public:
	Component(const std::string& compName) : name(compName) {}
	virtual ~Component() = default;

public:
	Object* GetOwner() const { return owner; }
	std::string GetName() const { return name; }

	void SetOwner(Object* obj) { owner = obj; }

public:
	virtual void Update() = 0;
	virtual void Render() = 0;

};