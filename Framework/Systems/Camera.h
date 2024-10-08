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

private:
	Matrix view, proj;
	unique_ptr<ViewProjectionBuffer> VPBuffer;

	Vector3 position = Vector3(0, 0, 0);

};

