#pragma once

class Object
{
protected:
	string name;
	map<string, shared_ptr<Component>> components;

	Vector3 position, scale;
	float rotation = 0.0f;

	Matrix world, S, R, T;
	unique_ptr<WorldBuffer> worldBuffer;

public:
	Object(const string& name, const Vector3& position, const Vector3& scale, const float rotation)
		: name(name), position(position), scale(scale), rotation(XMConvertToRadians(rotation))
	{
		worldBuffer = make_unique<WorldBuffer>();
	}

	Object(const Object& other)
		: Object(other.name, other.position, other.scale, other.rotation)
	{
		
	}
	
	Object operator=(const Object& other)
	{
		Object obj(other);
		return obj;
	}

	virtual ~Object() = default;

public:
	void AddComponent(const shared_ptr<Component>& component);	
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

