#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake()
{
	isLoaded = false;

	testPlayer = make_shared <PlayerUnit> (CENTER + Vector3(-550, -330, 0));
	
	lumberAxe = make_shared<LumberAxe>(CENTER + Vector3(-550, -330, 0));

	floor.push_back(make_shared<ColorLine>(CENTER + Vector3(-640, -330, 0), 10000, 0.0f, CLEAR));
	floor.back()->AddComponent(make_shared<FloorCollision>("FloorCollision", testPlayer, lumberAxe));
	floor[0]->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10000, 1, 0), GREEN));

	auto parallax = make_shared<ParallaxBackground>("Parallax", 5);

	// 배경 레이어 2개씩 생성
	for (int i = 0; i < 5; i++)
	{
		bgLayer[i] = make_shared<TextureRect>(CENTER + Vector3(0, 50, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/StartScene/" + to_wstring(i + 1) + L".png");
		bgLayerDup[i] = make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH, 50, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/StartScene/" + to_wstring(i + 1) + L".png");

		bgOrigin[i] = bgLayer[i]->GetPosition();
		bgOriginDup[i] = bgLayerDup[i]->GetPosition();

		parallax->SetLayer(i, bgLayer[i], bgLayerDup[i]);
	}

	//*
	//TileMap
	{
		UINT spacing = 50;
		UINT width = (WIN_DEFAULT_WIDTH * 2) / spacing;
		UINT height = (WIN_DEFAULT_HEIGHT * 2) / spacing;

		tmap = make_unique<TileMap>(width, height, spacing);
		tmap->Load("_Data/_TileData/map01.data");
	}
	//*/

	auto object01Collider = floor[0]->GetComponent<BoxCollider>("BoxCollider");

	if (object01Collider) COLLISION->AddCollider(object01Collider);

	backgroundObject = make_shared<Object>("Background");
	backgroundObject->AddComponent(parallax);

	isLoaded = true;
}

void Scene_03::Start()
{
	lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->SetTarget(testPlayer.get());
	lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->Recall();
}

void Scene_03::Destroy()
{
	for (auto& floors : floor) if (floors != nullptr) floors.reset();
	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();
	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup.reset();
	if (testPlayer != nullptr) testPlayer.reset();
	if (lumberAxe != nullptr) lumberAxe.reset();
	if (backgroundObject != nullptr) backgroundObject.reset();
}

void Scene_03::FixedUpdate() {}

void Scene_03::Update()
{
	if (tmap != nullptr) tmap->Update();

	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Update();

	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();

	for (const auto& floors : floor) if (floors != nullptr) floors->Update();

	if (testPlayer != nullptr) testPlayer->Update();

	if (lumberAxe != nullptr) lumberAxe->Update();

	if (backgroundObject != nullptr) backgroundObject->Update();

	CAMERA->UpdateHorizontalScroll(testPlayer.get(), 0.5f, 5.5f);

	
}

void Scene_03::LateUpdate() {}

void Scene_03::Render()
{
	if (backgroundObject != nullptr) backgroundObject->Render();

	if (tmap != nullptr) tmap->Render();

	for (const auto& floors : floor) if (floors != nullptr) floors->Render();

	if (testPlayer != nullptr) testPlayer->Render();

	if (lumberAxe != nullptr) lumberAxe->Render();
}

void Scene_03::PostRender()
{

}

void Scene_03::ImGUI()
{

}


