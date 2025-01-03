#pragma once

class GameObject
{
protected:
	string name;
	map<string, shared_ptr<GameComponent>> components;

	Vector3 position, scale;
	float rotation = 0.0f;

	Matrix world, S, R, T;
	unique_ptr<WorldBuffer> worldBuffer;

public:
	GameObject(const string& name, const Vector3& position, const Vector3& scale, const float rotation)
		: name(name), position(position), scale(scale), rotation(XMConvertToRadians(rotation))
	{
		worldBuffer = make_unique<WorldBuffer>();
	}

	GameObject(const GameObject& other)
		: GameObject(other.name, other.position, other.scale, other.rotation)
	{

	}

	GameObject operator=(const GameObject& other)
	{
		GameObject obj(other);
		return obj;
	}

	virtual ~GameObject() = default;

public:
	void AddComponent(const shared_ptr<GameComponent>& component);
	void RemoveComponent(const string& compName);

	template<typename T>
	shared_ptr<T> GetComponent(const string& compName) const
	{
		if (!components.count(compName)) return nullptr;

		return dynamic_pointer_cast<T>(components.at(compName));
	}

public:
	void Move(const Vector3& speed) { position += speed * DELTA; }
	void Rotate(const float rotation) { this->rotation += XMConvertToRadians(rotation) * DELTA; }
	void Scale(const Vector3& scale) { this->scale += scale * DELTA; }

public:
	virtual void Update();
	virtual void Render();

};

