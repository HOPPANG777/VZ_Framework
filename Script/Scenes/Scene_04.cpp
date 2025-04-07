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
	}

	// Floor
	{
		floor.push_back(make_shared<ColorLine>(CENTER + Vector3(-550, -55, 0), 0, 0.0f, CLEAR));
		floor[0]->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10000, 1, 0), GREEN));
	}

	// Floor Collider
	{
		for (const auto& flr : floor) if (flr != nullptr) flr->AddComponent(make_shared<FloorCollision>("FloorCollision", testPlayer, lumberAxe));
		for (const auto& flr : floor) if (flr != nullptr) COLLISION->AddCollider(flr->GetComponent<BoxCollider>("BoxCollider"));
	}

	// Background
	{
		auto parallax = make_shared<ParallaxBackground>("Parallax", 6);

		// 배경 레이어 2개씩 생성
		for (int i = 0; i < 5; i++)
		{
			bgLayer[i] = make_shared<TextureRect>(CENTER + Vector3(0, 200, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/Stage01/" + to_wstring(i + 1) + L".png");
			bgLayerDup[i] = make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH, 200, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/Stage01/" + to_wstring(i + 1) + L".png");

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

	{
		puzzleRock = make_shared<TextureRect>(CENTER + Vector3(-50, 45, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"Object/PuzzleRock.png");
		puzzleRock->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(200, 200, 0), BLACK));
		COLLISION->AddCollider(puzzleRock->GetComponent<BoxCollider>("BoxCollider"));
		puzzleRock->AddComponent(make_shared<PushableComponent>());
	}

	isLoaded = true;
}

void Scene_04::Start()
{
}

void Scene_04::Destroy()
{
	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();

	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup.reset();

	for (auto& floors : floor) if (floors != nullptr) floors.reset();
	
	if (backgroundObject != nullptr) backgroundObject.reset();

	if (tmap != nullptr) tmap.reset();

	if (puzzleRock != nullptr) puzzleRock.reset();

	if (testPlayer != nullptr) testPlayer.reset();

	if (lumberAxe != nullptr) lumberAxe.reset();
}

void Scene_04::FixedUpdate()
{
}

void Scene_04::Update()
{
	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Update();

	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();

	for (const auto& floors : floor) if (floors != nullptr) floors->Update();

	if (backgroundObject != nullptr) backgroundObject->Update();

	if (tmap != nullptr) tmap->Update();
	
	if (puzzleRock != nullptr) puzzleRock->Update();

	if (testPlayer != nullptr) testPlayer->Update();

	if (lumberAxe != nullptr) lumberAxe->Update();

	CAMERA->UpdateHorizontalScroll(testPlayer.get(), 0.5f, 2.0f);

}

void Scene_04::LateUpdate()
{
}

void Scene_04::Render()
{
	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Render();

	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Render();

	for (const auto& floors : floor) if (floors != nullptr) floors->Render();

	if (backgroundObject != nullptr) backgroundObject->Render();

	if (tmap != nullptr) tmap->Render();

	if (puzzleRock != nullptr) puzzleRock->Render();
	
	if (lumberAxe != nullptr) lumberAxe->Render();

	if (testPlayer != nullptr) testPlayer->Render();

}

void Scene_04::PostRender()
{
}

void Scene_04::ImGUI()
{
}
