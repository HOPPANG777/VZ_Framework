#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake()
{
	isLoaded = false;

	isNextStage = false;

	testPlayer = make_shared<PlayerUnit>(CENTER + Vector3(-550, -350, 0));
	
	lumberAxe = make_shared<LumberAxe>(CENTER + Vector3(-550, -330, 0));

	floor.push_back(make_shared<ColorLine>(CENTER + Vector3(-640, -330, 0), 10000, 0.0f, CLEAR));
	floor.back()->AddComponent(make_shared<FloorCollision>("FloorCollision", testPlayer, lumberAxe));
	floor[0]->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10000, 1, 0), GREEN));

	nextStagePoint = make_shared<Object>("NextStagePoint", CENTER + Vector3(3100, -330, 0), Vector3(Values::ZeroVec3), 0.0f);

	nextStageText = make_shared<Object>("NextStageText", CENTER + Vector3(0, -300, 0), Vector3(1000, 200, 0), 0.0f, TransformType::Rect);
	nextStageTextBG = make_shared<TextureRect>(CENTER + Vector3(2560, -300, 0), Vector3(1000, 250, 0), 0.0f, TexturePath + L"UI/Panel1.png");
	uiPlayerTexture = make_shared<TextureRect>(CENTER + Vector3(2100, -200, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"UI/Player_.png");
	nextStageText->AddComponent(make_shared<TypingText>());

	auto parallax = make_shared<ParallaxBackground>("Parallax", 5);

	// 배경 레이어 2개씩 생성
	for (int i = 0; i < 4; i++)
	{
		bgLayer[i] = make_shared<TextureRect>(CENTER + Vector3(0, 0, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/StartScene/" + to_wstring(i + 1) + L".png");
		bgLayerDup[i] = make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH, 0, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/StartScene/" + to_wstring(i + 1) + L".png");

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

	nextStagePoint->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 100, 0), BLACK));
	COLLISION->AddCollider(nextStagePoint->GetComponent<BoxCollider>("BoxCollider"));

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
	if (nextStagePoint != nullptr) nextStagePoint.reset();
	if (nextStageText != nullptr) nextStageText.reset();
	if (nextStageTextBG != nullptr) nextStageTextBG.reset();
	if (uiPlayerTexture != nullptr) uiPlayerTexture.reset();
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

	if (nextStagePoint != nullptr) nextStagePoint->Update();

	if (nextStageTextBG != nullptr) nextStageTextBG->Update();

	if (uiPlayerTexture != nullptr) uiPlayerTexture->Update();

	CAMERA->UpdateHorizontalScroll(testPlayer.get(), 0.5f, 2.5f);

	if (nextStagePoint->GetComponent<BoxCollider>("BoxCollider") ->IsColliding(testPlayer->GetComponent<BoxCollider>("BoxCollider").get()))
	{
		isNextStage = true;
		if (nextStageText != nullptr) nextStageText->Update();
		if (INPUT->KeyDown(VK_RETURN))
		{
			SCENE->LoadScene("Stage01");
		}
	}
}

void Scene_03::LateUpdate() {}

void Scene_03::Render()
{
	if (backgroundObject != nullptr) backgroundObject->Render();

	if (tmap != nullptr) tmap->Render();

	for (const auto& floors : floor) if (floors != nullptr) floors->Render();

	if (lumberAxe != nullptr) lumberAxe->Render();

	if (testPlayer != nullptr) testPlayer->Render();

	if (nextStagePoint != nullptr) nextStagePoint->Render();

	if (isNextStage == true)
	{
		if (uiPlayerTexture != nullptr) uiPlayerTexture->Render();
		if (nextStageTextBG != nullptr) nextStageTextBG->Render();
	}
}

void Scene_03::PostRender()
{
	nextStageText->GetComponent<TypingText>("TypingText")->RenderText(L"이제 이곳을 떠날 때가 되었나? (Enter를 입력해주세요.)", nextStageText->GetRect(), 25.0f, BLACK);
}

void Scene_03::ImGUI()
{

}


