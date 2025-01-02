#pragma once

class Camera
{
	DECLARE_SINGLETON(Camera);
public:
	void Init();

	void Update();
	void Render();

	void UpdateView();
	void Move();
	const Vector3 Drag();

	Vector3 GetPosition() const { return position; }
	void SetPosition(const Vector3& position) { this->position = position; }


private:
	Matrix view, proj;
	unique_ptr<ViewProjectionBuffer> VPBuffer;

	Vector3 position = Values::ZeroVec3;

	bool isMove = false;
	float moveSpeed = 100.0f;
};

