#pragma once

class Camera
{
	DECLARE_SINGLETON(Camera);
public:
	void Init();

	void Update();
	void Render();

public:
	void UpdateView();
	void Move();
	const Vector3 Drag();

	void Reset();

public:
	Vector3 GetPosition() const { return position; }
	void SetPosition(const Vector3& position) { this->position = position; }

	void SetTarget(Object* target) { this->target = target; }

	Object* GetTarget() { return target; }

private:
	Matrix view, proj;
	unique_ptr<ViewProjectionBuffer> VPBuffer;

	Vector3 position = Values::ZeroVec3;

	Object* target = nullptr;

	bool isMove = false;
	float moveSpeed = 100.0f;
};

