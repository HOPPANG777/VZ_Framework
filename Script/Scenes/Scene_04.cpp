#include "stdafx.h"
#include "Scene_04.h"

void Scene_04::Awake()
{
	isLoaded = false;

	/*
	// Floor
	{
		floor.push_back(make_shared<ColorLine>(CENTER + Vector3(-640, -330, 0), 10000, 0.0f, CLEAR));
		floor[0]->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10000, 1, 0), GREEN));
	}

	// Floor Collider
	{
		for (const auto& flr : floor) if (flr != nullptr) flr->AddComponent(make_shared<FloorCollision>("FloorCollision", testPlayer, lumberAxe));
		for (const auto& flr : floor) if (flr != nullptr) COLLISION->AddCollider(flr->GetComponent<BoxCollider>("BoxCollider"));
	}

	// Background
	{
		auto parallax = make_shared<ParallaxBackground>("Parallax", 5);

		// 배경 레이어 2개씩 생성
		for (int i = 0; i < 4; i++)
		{
			bgLayer[i] = make_shared<TextureRect>(CENTER + Vector3(0, 0, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/Stage01/" + to_wstring(i + 1) + L".png");
			bgLayerDup[i] = make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH, 0, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/Stage01/" + to_wstring(i + 1) + L".png");

			bgOrigin[i] = bgLayer[i]->GetPosition();
			bgOriginDup[i] = bgLayerDup[i]->GetPosition();

			parallax->SetLayer(i, bgLayer[i], bgLayerDup[i]);
		}

		backgroundObject = make_shared<Object>("Background");
		backgroundObject->AddComponent(parallax);
	}

	//TileMap
	{
		UINT spacing = 50;
		UINT width = (WIN_DEFAULT_WIDTH * 2) / spacing;
		UINT height = (WIN_DEFAULT_HEIGHT * 2) / spacing;

		tmap = make_unique<TileMap>(width, height, spacing);
		tmap->Load("_Data/_TileData/map02.data");
	}
	*/

	isLoaded = true;
}

void Scene_04::Start()
{
}

void Scene_04::Destroy()
{
	/*
	for (auto& floors : floor) if (floors != nullptr) floors.reset();

	//for (auto& house : house) if (house != nullptr) house.reset();

	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();

	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup.reset();

	if (backgroundObject != nullptr) backgroundObject.reset();
	*/
}

void Scene_04::FixedUpdate()
{
}

void Scene_04::Update()
{
	/*
	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Update();

	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();

	for (const auto& floors : floor) if (floors != nullptr) floors->Update();

	//for (const auto& house : house) if (house != nullptr) house->Update();

	if (backgroundObject != nullptr) backgroundObject->Update();
	
	if (tmap != nullptr) tmap->Update();
	*/
}

void Scene_04::LateUpdate()
{
}

void Scene_04::Render()
{
	/*
	if (backgroundObject != nullptr) backgroundObject->Render();

	if (tmap != nullptr) tmap->Render();

	for (const auto& floors : floor) if (floors != nullptr) floors->Render();

	//if (house[1] != nullptr) house[1]->Render();

	//if (house[2] != nullptr) house[2]->Render();

	//if (house[3] != nullptr) house[3]->Render();

	//if (testPlayer != nullptr) testPlayer->Render();

	//if (house[0] != nullptr) house[0]->Render();
	*/
}

void Scene_04::PostRender()
{
}

void Scene_04::ImGUI()
{
}
