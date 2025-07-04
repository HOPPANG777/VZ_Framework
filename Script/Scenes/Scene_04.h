#pragma once

#include "Component/ParallaxBackground.h"
#include "Component/FloorCollision.h"
#include "Component/PushableComponent.h"
#include "GameObject/PlayerUnit.h"
#include "GameObject/LumberAxe.h"
#include "GameObject/Wind.h"
#include "Component/WindZone.h"
#include "GameObject/DialogTextBox.h"
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

	std::shared_ptr<GameObject> testPlayer;
	std::shared_ptr<GameObject> lumberAxe;

	std::vector<std::shared_ptr<GameObject>> floor;
	std::vector<std::shared_ptr<GameObject>> text;
	std::vector<std::shared_ptr<GameObject>> textPoint;

	std::shared_ptr<GameObject> puzzleRock;
	std::shared_ptr<GameObject> bridge;

	std::shared_ptr<GameObject> windObject;

	std::shared_ptr<GameObject> backgroundObject;

	std::shared_ptr<GameObject> bgLayer[5];
	std::shared_ptr<GameObject> bgLayerDup[5];
	Vector3 bgOrigin[5];
	Vector3 bgOriginDup[5];

	std::unique_ptr<TileMap> tmap;

};

