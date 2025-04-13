#pragma once

#include "Component/ParallaxBackground.h"
#include "Component/FloorCollision.h"
#include "Object/PlayerUnit.h"
#include "Object/LumberAxe.h"
#include "Component/LadderComponent.h"
#include "Component/LadderTransComponent.h"
#include "Object/DialogTextBox.h"
#include "Component/WallCollision.h"

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

	shared_ptr<Object> testPlayer;
	shared_ptr<Object> lumberAxe;

	vector<shared_ptr<Object>> floor;
	vector<shared_ptr<Object>> wall;
	vector<shared_ptr<Object>> text;
	vector<shared_ptr<Object>> textPoint;

	shared_ptr<Object> ladder;
	shared_ptr<Object> ladder2;

	shared_ptr<Object> backgroundObject;

	shared_ptr<Object> bgLayer[5];
	shared_ptr<Object> bgLayerDup[5];
	Vector3 bgOrigin[5];
	Vector3 bgOriginDup[5];

	unique_ptr<TileMap> tmap;

};

