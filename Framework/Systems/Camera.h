#pragma once
#include "Utilities/DirectHelper.h"

class GameObject;
class ViewProjectionBuffer;

class Camera
{
	DECLARE_SINGLETON(Camera);

public:
	void Init();

	void Update();
	void Render();

public:
	void SetProjection(const float width, const float height);
	
	void UpdateView();
	void Move();
	const Vector3 Drag();

	void Reset();

public:
	Vector3 GetPosition() const { return position; }
	void SetPosition(const Vector3& position) { this->position = position; }

	void SetTarget(GameObject* target) { this->cameraTarget = target; }
	void RemoveTarget() { this->cameraTarget = nullptr; }

	GameObject* GetTarget() { return cameraTarget; }

public:
	void UpdateFollow();
	void UpdateHorizontalScroll(GameObject* target, float minRatio = 0.5f, float maxRatio = 2.5f);
	void UpdateVerticalScroll(GameObject* target, float minRatio = 0.5f, float maxRatio = 2.5f);

private:
	Matrix view, proj;
	std::unique_ptr<ViewProjectionBuffer> VPBuffer;

	Vector3 position = Values::ZeroVec3;

	GameObject* cameraTarget = nullptr;

	bool isMove = false;
	float moveSpeed = 150.0f;

	float accelerationTime = 0.0f;
	float currentSpeed = 0.0f; 

	float prevWidth = 0.0f;
	float prevHeight = 0.0f;
};

