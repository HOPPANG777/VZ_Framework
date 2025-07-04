#pragma once

#include "Interfaces/IScene.h"
#include "Component/ParallaxBackground.h"

class Scene_06 : public Scene
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
	bool isLoaded = false;

	std::shared_ptr<GameObject> bgLayer[5];
	std::shared_ptr<GameObject> bgLayerDup[5];
	Vector3 bgOrigin[5];
	Vector3 bgOriginDup[5];

};

