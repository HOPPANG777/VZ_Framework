#pragma once

#include "Interfaces/IScene.h"
#include "Objects/TextureObject/TextureRect.h"
#include "Components/Button.h"
#include "Components/Texts.h"
#include "Managers/SceneManager.h"

class Scene_01 : public Scene
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
	std::vector<std::shared_ptr<GameObject>> objects;

	std::shared_ptr<GameObject> startButton;

	bool isLoaded = false;
};

