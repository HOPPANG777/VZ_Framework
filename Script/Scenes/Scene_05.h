#pragma once

#include "Component/ParallaxBackground.h"
#include "Component/FloorCollision.h"
#include "GameObject/PlayerUnit.h"
#include "GameObject/LumberAxe.h"
#include "Component/LadderComponent.h"
#include "Component/LadderTransComponent.h"
#include "GameObject/DialogTextBox.h"
#include "Component/WallCollision.h"
#include "Objects/TileObject/TileMap.h"
#include "Objects/ColorObject/ColorLine.h"

class Scene_05 : public Scene
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
	bool isTest2 = false;

	std::shared_ptr<GameObject> testPlayer;
	std::shared_ptr<GameObject> lumberAxe;

	std::shared_ptr<GameObject> bback;

	std::vector<std::shared_ptr<GameObject>> floor;
	std::vector<std::shared_ptr<GameObject>> wall;
	std::vector<std::shared_ptr<GameObject>> text;
	std::vector<std::shared_ptr<GameObject>> textPoint;

	std::shared_ptr<GameObject> ladder;
	std::shared_ptr<GameObject> ladder2;

	std::shared_ptr<GameObject> backgroundObject;

	std::shared_ptr<GameObject> bgLayer[5];
	std::shared_ptr<GameObject> bgLayerDup[5];
	Vector3 bgOrigin[5];
	Vector3 bgOriginDup[5];

	std::unique_ptr<TileMap> tmap;

};

