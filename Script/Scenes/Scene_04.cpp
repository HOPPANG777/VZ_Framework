#include "stdafx.h"
#include "Scene_04.h"

void Scene_04::Awake()
{
	isLoaded = false;

	// Player
	{
		testPlayer = make_shared<PlayerUnit>(CENTER + Vector3(-550, -40, 0));
		lumberAxe = make_shared<LumberAxe>(CENTER + Vector3(-550, -30, 0));
		lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->SetTarget(testPlayer.get());
		SCENE->SetPlayer(testPlayer);
	}

	// Floor
	{
		floor.push_back(make_shared<ColorLine>(CENTER + Vector3(-550, -60, 0), 0, 0.0f, CLEAR));
		floor[0]->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10000, 1, 0), GREEN));
	}

	// Floor Collider
	{
		for (const std::shared_ptr<Object>& flr : floor) if (flr != nullptr) flr->AddComponent(make_shared<FloorCollision>("FloorCollision", testPlayer, lumberAxe));
		for (const std::shared_ptr<Object>& flr : floor) if (flr != nullptr) COLLISION->AddCollider(flr->GetComponent<BoxCollider>("BoxCollider"));
	}

	// Background
	{
		std::shared_ptr<ParallaxBackground> parallax = make_shared<ParallaxBackground>("Parallax", 6);

		// 배경 레이어 2개씩 생성
		for (int i = 0; i < 5; i++)
		{
			bgLayer[i] = make_shared<TextureRect>(CENTER + Vector3(0, 0, 0), Vector3(WIN_DEFAULT_WIDTH * 1.2f, WIN_DEFAULT_HEIGHT * 1.2f, 0), 0.0f, TexturePath + L"Background/StartScene/" + to_wstring(i + 1) + L".png");
			bgLayerDup[i] = make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH * 1.2f, 0, 0), Vector3(WIN_DEFAULT_WIDTH * 1.2f, WIN_DEFAULT_HEIGHT * 1.2f, 0), 0.0f, TexturePath + L"Background/StartScene/" + to_wstring(i + 1) + L".png");

			bgOrigin[i] = bgLayer[i]->GetPosition();
			bgOriginDup[i] = bgLayerDup[i]->GetPosition();

			parallax->SetLayer(i, bgLayer[i], bgLayerDup[i]);
		}

		backgroundObject = make_shared<Object>("Background");
		backgroundObject->AddComponent(parallax);
	}

	// TileMap
	{
		UINT spacing = 50;
		UINT width = (WIN_DEFAULT_WIDTH * 2) / spacing;
		UINT height = (WIN_DEFAULT_HEIGHT * 2) / spacing;

		tmap = make_unique<TileMap>(width, height, spacing);
		tmap->Load("_Data/_TileData/map02.data");
	}

	// Puzzle Rock
	{
		puzzleRock = make_shared<TextureRect>(CENTER + Vector3(-50, 45, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"Object/PuzzleRock.png");
		puzzleRock->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(200, 200, 0), BLACK));
		COLLISION->AddCollider(puzzleRock->GetComponent<BoxCollider>("BoxCollider"));
		puzzleRock->AddComponent(make_shared<PushableComponent>());
	}

	// Objects
	{
		objects.push_back(make_shared<TextureRect>(CENTER + Vector3(360, 0, 0), Vector3(600, 200, 0), 0.0f, TexturePath + L"Object/ObjectBridge.png"));
	}

	// Wind
	{
		// 예시: 플레이어를 뒤로 밀어내는 바람 영역 생성 
		// 예를 들어, x축 기준으로 300~500 사이, y축은 전체, z는 0 (2D 게임이라 가정)
		Vector3 windRegionMin = { 45, -1000, 0 };
		Vector3 windRegionMax = { 600, 1000, 0 };
		// 바람 방향: 플레이어를 왼쪽으로 밀어낸다고 하면, (-1, 0, 0)
		Vector3 windDirection = { -1, 0, 0 };
		float windIntensity = 220.0f; // 적절한 값 조절
		
		windObject = make_shared<Wind>(CENTER + Vector3(360, 0, 0), Vector3(600, 600, 0), 0.0f);
		windObject->AddComponent(make_shared<WindZone>("WindZone", CENTER + windRegionMin, CENTER + windRegionMax, windDirection, windIntensity));
	}

	isLoaded = true;
}

void Scene_04::Start()
{
}

void Scene_04::Destroy()
{
	if (tmap != nullptr) tmap.reset();

	if (backgroundObject != nullptr) backgroundObject.reset();

	if (puzzleRock != nullptr) puzzleRock.reset();

	if (lumberAxe != nullptr) lumberAxe.reset();

	if (testPlayer != nullptr) testPlayer.reset();

	if (windObject != nullptr) windObject.reset();

	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();
		 
	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup.reset();
		 
	for (auto& floors : floor) if (floors != nullptr) floors.reset();
		 
	for (auto& obj : objects) if (obj != nullptr) obj.reset();

	SCENE->ResetPlayer();
}

void Scene_04::FixedUpdate()
{
}

void Scene_04::Update()
{
	if (tmap != nullptr) tmap->Update();
	if (backgroundObject != nullptr) backgroundObject->Update();
	if (puzzleRock != nullptr) puzzleRock->Update();
	if (lumberAxe != nullptr) lumberAxe->Update();
	if (testPlayer != nullptr) testPlayer->Update();
	if (windObject != nullptr) windObject->Update();
	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Update();
	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();
	for (const auto& floors : floor) if (floors != nullptr) floors->Update();
	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
	CAMERA->UpdateHorizontalScroll(testPlayer.get(), 0.5f, 1.5f);
}

void Scene_04::LateUpdate()
{
}

void Scene_04::Render()
{
	if (backgroundObject != nullptr) backgroundObject->Render();
	if (tmap != nullptr) tmap->Render();
	for (const auto& floors : floor) if (floors != nullptr) floors->Render();
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
	if (puzzleRock != nullptr) puzzleRock->Render();
	if (lumberAxe != nullptr) lumberAxe->Render();
	if (testPlayer != nullptr) testPlayer->Render();
	if (windObject != nullptr) windObject->Render();
}

void Scene_04::PostRender()
{
}

void Scene_04::ImGUI()
{
}
