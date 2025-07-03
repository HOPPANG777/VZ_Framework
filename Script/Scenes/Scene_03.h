#pragma once

#include "Object/PlayerUnit.h"
#include "Object/LumberAxe.h"
#include "Component/ParallaxBackground.h"
#include "Component/FloorCollision.h"
#include "Object/DialogTextBox.h"
#include "Component/LogPathCreator.h"
#include "Component/SolidCollider.h"
#include "Interfaces/IScene.h"
#include "Objects/TileObject/TileMap.h"
#include "Objects/ColorObject/ColorLine.h"
#include "Managers/SceneManager.h"

class Scene_03 : public Scene
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
	bool isNextStage = false;
	bool isTest = false;
	bool isTest2 = false;

	std::vector<std::shared_ptr<Object>> floor;
	std::vector<std::shared_ptr<Object>> house;
	std::vector<std::shared_ptr<Object>> text;
	std::vector<std::shared_ptr<Object>> textPoint;
	std::vector<std::shared_ptr<Object>> log;

	std::shared_ptr<Object> lumberAxe;
	std::shared_ptr<Object> testPlayer;

	std::shared_ptr<Object> backgroundObject;

	std::shared_ptr<Object> bgLayer[5];
	std::shared_ptr<Object> bgLayerDup[5];
	Vector3 bgOrigin[5];
	Vector3 bgOriginDup[5];

	std::unique_ptr<TileMap> tmap;

};

