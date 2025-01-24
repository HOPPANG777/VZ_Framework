#pragma once

class Object
{
protected:
	string name;
	map<string, shared_ptr<Component>> components;

	unique_ptr<WorldBuffer> worldBuffer;

public:
	// 기본 생성자 (Object 이름만 지정)
	Object(const std::string& name);

	// 위치/스케일/회전을 초기에 지정하는 생성자
	Object(const std::string& name, const Vector3& initPos, const Vector3& initScale, float initRot);

	// 복사 생성/대입 연산자
	Object(const Object& other);
	Object& operator=(const Object& other)
	{
		if (this != &other)
		{
			name = other.name;
			// 필요한 경우 components를 복제
		}
		return *this;
	}

	virtual ~Object() = default;

public:
	void AddComponent(const std::shared_ptr<Component>& component);
	void RemoveComponent(const std::string& compName);

	template<typename T>
	shared_ptr<T> GetComponent(const string& compName) const
	{
		if (!components.count(compName)) return nullptr;
		return dynamic_pointer_cast<T>(components.at(compName));
	}
	
public:
	// 매 프레임 업데이트 & 렌더
	virtual void Update();
	virtual void Render();

public:
	// Transform 컴포넌트에 대한 편의 함수들
	Vector3 GetPosition() const;
	Vector3 GetScale() const;
	float   GetRotation() const;

public:
	void Move(const Vector3& speed);
	void Rotate(float rot);
	void Scale(const Vector3& scale);

};

