#pragma once

class Scene
{
public:
	virtual ~Scene() = default;

	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Destroy() = 0;

	virtual void FixedUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;

	virtual void Render() = 0;
	virtual void PostRender() = 0;

	virtual void ImGUI() = 0;

	virtual bool IsLoaded() const = 0;
};

