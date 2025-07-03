#pragma once
#include "Objects/BaseObject/Object.h"
#include "Utilities/DirectHelper.h"
//#include "Renders/RenderResources/GlobalBuffers.h"
#include "Systems/Input.h"
#include "Systems/Time.h"

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
	void RemoveTarget() { this->target = nullptr; }

	Object* GetTarget() { return target; }

public:
	void UpdateFollow();
	void UpdateHorizontalScroll(Object* target, float minRatio = 0.5f, float maxRatio = 2.5f);
	void UpdateVerticalScroll(Object* target, float minRatio = 0.5f, float maxRatio = 2.5f);

private:
	Matrix view, proj;
	unique_ptr<ViewProjectionBuffer> VPBuffer;

	Vector3 position = Values::ZeroVec3;

	Object* target = nullptr;

	bool isMove = false;
	float moveSpeed = 150.0f;

	float accelerationTime = 0.0f;
	float currentSpeed = 0.0f; 
};

