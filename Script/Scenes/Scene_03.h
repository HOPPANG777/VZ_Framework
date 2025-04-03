#pragma once

#include "Object/PlayerUnit.h"
#include "Object/LumberAxe.h"
#include "Component/ParallaxBackground.h"
#include "Component/FloorCollision.h"
#include "Object/DialogTextBox.h"

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

	vector<shared_ptr<Object>> floor;
	vector<shared_ptr<Object>> house;
	vector<shared_ptr<Object>> text;
	vector<shared_ptr<Object>> textPoint;

	shared_ptr<Object> player;
	shared_ptr<Object> lumberAxe;

	shared_ptr<Object> backgroundObject;

	shared_ptr<Object> bgLayer[5];
	shared_ptr<Object> bgLayerDup[5];
	Vector3 bgOrigin[5];
	Vector3 bgOriginDup[5];

	unique_ptr<TileMap> tmap;

	shared_ptr<Object> testPlayer;

	Vector3 testPlayerVelocity = Vector3::Zero; // testPlayer의 수직(및 필요시 전체) 속도

};

