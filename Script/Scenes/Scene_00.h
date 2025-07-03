#pragma once

#include "Object/JumpHoppang.h"
#include "Components/Texts.h"
#include "Objects/ColorObject/ColorRect.h"
#include "Interfaces/IScene.h"
#include "Systems/Camera.h"

class Scene_00 : public Scene
{
public:
	// Scene을(를) 통해 상속됨
	void Awake() override;
	void Start() override;

	void Destroy() override;

	void FixedUpdate() override;
	void Update() override;
	void LateUpdate() override;

	void Render() override;
	void PostRender() override;

	void ImGUI() override;

	bool IsLoaded() const override { return isLoaded; }

private:
	std::vector<std::shared_ptr<Object>> objects;

	std::shared_ptr<Object> LoadingTextObject;

	bool isLoaded = false;
};
