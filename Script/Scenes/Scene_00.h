#pragma once

#include "Interfaces/IScene.h"
#include "Objects/TextureObject/TextureRect.h"
#include "Objects/TextureObject/AnimationRect.h"
#include "Objects/ColorObject/ColorRect.h"

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

private:
	vector<shared_ptr<Object>> objects;

};
