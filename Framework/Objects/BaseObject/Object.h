#pragma once

enum class TransformType 
{
	Normal,   // 일반 게임 월드용 Transform
	Rect      // UI용 RectTransform
};

class Object
{
protected:
	string name;
	map<string, shared_ptr<Component>> components;

	unique_ptr<WorldBuffer> worldBuffer;

public:
	// 기본 생성자 (Object 이름만 지정)
	Object(const string& name, TransformType type = TransformType::Normal);

	// 위치/스케일/회전을 초기에 지정하는 생성자
	Object(const string& name, const Vector3& initPos, const Vector3& initScale, float initRot, TransformType type = TransformType::Normal);

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
	void AddComponent(const shared_ptr<Component>& component);
	void RemoveComponent(const string& compName);

	template<typename T>
	shared_ptr<T> GetComponent(const string& compName) const
	{
		if (!components.count(compName)) return nullptr;
		return dynamic_pointer_cast<T>(components.at(compName));
	}
	
public:
	// GetName 함수 추가
	string GetName() const { return name; }
	void SetName(const string& newName) { name = newName; }

public:
	// Helper 함수 추가: 각 Transform 컴포넌트를 반환
	shared_ptr<Transform> GetTransform() const;
	shared_ptr<RectTransform> GetRectTransform() const;

public:
	// 매 프레임 업데이트 & 렌더
	virtual void Update();
	virtual void Render();

public:
	// Transform 컴포넌트에 대한 편의 함수들
	Vector3 GetPosition() const;
	Vector3 GetScale() const;
	float GetRotation() const;

public:
	void SetPosition(const Vector3& position);
	void SetScale(const Vector3& scale);
	void SetRotation(const float& rotation);

public:
	RECT GetRect();
	RECT GetCorrectedRect();

public:
	void Move(const Vector3& speed);
	void Rotate(float rot);
	void Scale(const Vector3& scale);

public:
	template<typename T>
	T* FindComponentByInterface() const
	{
		try
		{
			// 컨테이너가 비어있는지 먼저 확인
			if (components.empty())
			{
				return nullptr;
			}

			for (const auto& pair : components)
			{
				// pair.second는 std::unique_ptr<Component> 등이라고 가정
				if (T* comp = dynamic_cast<T*>(pair.second.get()))
				{
					return comp;
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "[FindComponentByInterface] 예외 발생: " << e.what() << std::endl;
			// 필요한 경우 추가적인 복구 또는 로그 처리
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetAllComponentsByInterface() const
	{
		std::vector<T*> result;
		try
		{
			if (components.empty())
			{
				return result;
			}

			for (const auto& pair : components)
			{
				if (T* comp = dynamic_cast<T*>(pair.second.get()))
				{
					result.push_back(comp);
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "[GetAllComponentsByInterface] 예외 발생: " << e.what() << std::endl;
		}
		return result;
	}
};

