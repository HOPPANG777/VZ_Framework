#pragma once

class GameObject;
class GameComponent
{
protected:
	string name;
	GameObject* owner = nullptr;

public:
	GameComponent(const string& compName) : name(compName) {}
	virtual ~GameComponent() = default;

public:
	GameObject* GetOwner() const { return owner; }
	string GetName() const { return name; }

	void SetOwner(GameObject* obj) { owner = obj; }

public:
	virtual void Update() = 0;
	virtual void Render() = 0;
};