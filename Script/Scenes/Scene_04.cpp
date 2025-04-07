#include "stdafx.h"
#include "Scene_04.h"

void Scene_04::Awake()
{
	isLoaded = false;

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

	if (backgroundObject != nullptr) backgroundObject.reset();

	if (tmap != nullptr) tmap.reset();

	if (puzzleRock != nullptr) puzzleRock.reset();
}

void Scene_04::FixedUpdate()
{
}

void Scene_04::Update()
{
	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Update();

	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();

	if (backgroundObject != nullptr) backgroundObject->Update();

	if (tmap != nullptr) tmap->Update();
	
	if (puzzleRock != nullptr) puzzleRock->Update();
}

void Scene_04::LateUpdate()
{
}

void Scene_04::Render()
{
	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Render();

	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Render();

	if (backgroundObject != nullptr) backgroundObject->Render();

	if (tmap != nullptr) tmap->Render();

	if (puzzleRock != nullptr) puzzleRock->Render();
}

void Scene_04::PostRender()
{
}

void Scene_04::ImGUI()
{
}
