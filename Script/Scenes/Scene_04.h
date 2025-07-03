#pragma once

#include "Component/ParallaxBackground.h"
#include "Component/FloorCollision.h"
#include "Component/PushableComponent.h"
#include "Object/PlayerUnit.h"
#include "Object/LumberAxe.h"
#include "Object/Wind.h"
#include "Component/WindZone.h"
#include "Object/DialogTextBox.h"
#include "Objects/ColorObject/ColorLine.h"
#include "Systems/Sounds.h"
#include "Objects/TileObject/TileMap.h"

class Scene_04 : public Scene
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
	bool isTest = false;
	bool isTest1 = false;

	std::shared_ptr<Object> testPlayer;
	std::shared_ptr<Object> lumberAxe;

	std::vector<std::shared_ptr<Object>> floor;
	std::vector<std::shared_ptr<Object>> text;
	std::vector<std::shared_ptr<Object>> textPoint;

	std::shared_ptr<Object> puzzleRock;
	std::shared_ptr<Object> bridge;

	std::shared_ptr<Object> windObject;

	std::shared_ptr<Object> backgroundObject;

	std::shared_ptr<Object> bgLayer[5];
	std::shared_ptr<Object> bgLayerDup[5];
	Vector3 bgOrigin[5];
	Vector3 bgOriginDup[5];

	std::unique_ptr<TileMap> tmap;

};

