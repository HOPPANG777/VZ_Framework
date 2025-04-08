#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake()
{
	isLoaded = false;
	
	// Player
	{
		testPlayer = make_shared<PlayerUnit>(CENTER + Vector3(-550, -450, 0));
		lumberAxe = make_shared<LumberAxe>(CENTER + Vector3(-550, -330, 0));
		lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->SetTarget(testPlayer.get());
	}

	// Floor
	{
		floor.push_back(make_shared<ColorLine>(CENTER + Vector3(5800, -320, 0), 0, 0.0f, CLEAR));
		floor[0]->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10000, 1, 0), GREEN));
		floor.push_back(make_shared<ColorLine>(CENTER + Vector3(-170, -320, 0), 0, 0.0f, CLEAR));
		floor[1]->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(950, 1, 0), GREEN));
	}

	// Floor Collider
	{
		for (const auto& flr : floor) if (flr != nullptr) flr->AddComponent(make_shared<FloorCollision>("FloorCollision", testPlayer, lumberAxe));
		for (const auto& flr : floor) if (flr != nullptr) COLLISION->AddCollider(flr->GetComponent<BoxCollider>("BoxCollider"));
	}
	
	// House
	{
		house.push_back(make_shared<TextureRect>(CENTER + Vector3(-450, -180, 0), Vector3(300, 300, 0), 0.0f, TexturePath + L"Object/House01.png"));
		house.push_back(make_shared<TextureRect>(CENTER + Vector3(-450, -180, 0), Vector3(300, 300, 0), 0.0f, TexturePath + L"Object/House02.png"));
		house.push_back(make_shared<TextureRect>(CENTER + Vector3(-450, -180, 0), Vector3(300, 300, 0), 0.0f, TexturePath + L"Object/House03.png"));
		house.push_back(make_shared<TextureRect>(CENTER + Vector3(-300, -280, 0), Vector3(70, 70, 0), 0.0f, TexturePath + L"Object/Wood.png"));
		house.push_back(make_shared<TextureRect>(CENTER + Vector3(1300, -190, 0), Vector3(250, 250, 0), 0.0f, TexturePath + L"Object/ObjectHouse01.png"));
		house.push_back(make_shared<TextureRect>(CENTER + Vector3(2100, -190, 0), Vector3(250, 250, 0), 0.0f, TexturePath + L"Object/ObjectHouse02.png"));
	}
	
	// Text
	{
		text.push_back(make_shared<DialogTextBox>("Text_01", CENTER + Vector3(0, -300, 0), CENTER + Vector3(0, -300, 0), Vector3(640, 200, 0), 0.0f, TexturePath + L"UI/Panel1.png", TexturePath + L"UI/Arrow.png", L"제100회팔씨름챔피언선발토너먼트 선수 등록 마감 까지 일주일... 선수 등록을 진행하는 도시까지 전속력으로 달린다!", 25.0f, L"마비옛체_OTF", BLACK));
		textPoint.push_back(make_shared<Object>("TextPoint_01", CENTER + Vector3(-300, -300, 0), Vector3(Values::ZeroVec3), 0.0f));
		text.push_back(make_shared<DialogTextBox>("Text_02", CENTER + Vector3(0, -300, 0), CENTER + Vector3(1920, -300, 0), Vector3(640, 200, 0), 0.0f, TexturePath + L"UI/Panel1.png", TexturePath + L"UI/Arrow.png", L"이제 이곳을 떠날 때가 되었나? (Enter를 입력해주세요.)", 25.0f, L"마비옛체_OTF", BLACK));
		textPoint.push_back(make_shared<Object>("TextPoint_02", CENTER + Vector3(2400, -330, 0), Vector3(Values::ZeroVec3), 0.0f));
	}

	// TextPoint Collider
	{
		for (const auto& textPoint : textPoint) textPoint->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 100, 0), BLACK));
		for (const auto& textPoint : textPoint) COLLISION->AddCollider(textPoint->GetComponent<BoxCollider>("BoxCollider"));
	}
	
	// Background
	{
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

		backgroundObject = make_shared<Object>("Background");
		backgroundObject->AddComponent(parallax);
	}

	// TileMap
	{
		UINT spacing = 50;
		UINT width = (WIN_DEFAULT_WIDTH * 2) / spacing;
		UINT height = (WIN_DEFAULT_HEIGHT * 2) / spacing;

		tmap = make_unique<TileMap>(width, height, spacing);
		tmap->Load("_Data/_TileData/map01.data");
	}

	// Log
	{
		log.push_back(make_shared<TextureRect>(CENTER + Vector3(260, -65, 0), Vector3(300, 500, 0), 0.0f, TexturePath + L"Object/Log.png"));
		log[0]->AddComponent(make_shared<HitComponent>("HitComponent"));
		log[0]->AddComponent(make_shared<LogPathCreator>());
		log[0]->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 500, 0), BLACK));
		COLLISION->AddCollider(log[0]->GetComponent<BoxCollider>("BoxCollider"));
	}

	isLoaded = true;
}

void Scene_03::Start()
{
	lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->Recall();
	dynamic_pointer_cast<DialogTextBox>(text[0])->Deactivate();
	dynamic_pointer_cast<DialogTextBox>(text[1])->Deactivate();
}

void Scene_03::Destroy()
{
	for (auto& floors : floor) if (floors != nullptr) floors.reset();

	for (auto& house : house) if (house != nullptr) house.reset();

	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();

	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup.reset();

	for (auto& text : text) if (text != nullptr) text.reset();
	
	for (auto& textPoint : textPoint) if (textPoint != nullptr) textPoint.reset();

	for (auto& log : log) if (log != nullptr) log.reset();

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

	for (const auto& house : house) if (house != nullptr) house->Update();

	for (const auto& text : text) if (text != nullptr) text->Update();

	for (const auto& textPoint : textPoint) if (textPoint != nullptr) textPoint->Update();

	for (const auto& logs : log) if (logs != nullptr) logs->Update();

	if (testPlayer != nullptr) testPlayer->Update();

	if (lumberAxe != nullptr) lumberAxe->Update();

	if (backgroundObject != nullptr) backgroundObject->Update();

	CAMERA->UpdateHorizontalScroll(testPlayer.get(), 0.5f, 2.0f);

	if (textPoint[0]->GetComponent<BoxCollider>("BoxCollider")->IsColliding(testPlayer->GetComponent<BoxCollider>("BoxCollider").get()) && text[0]->GetComponent<TypingText>("TypingText")->GetTextOut() == false)
	{
		dynamic_pointer_cast<DialogTextBox>(text[0])->Activate();
	}

	if (textPoint[1]->GetComponent<BoxCollider>("BoxCollider")->IsColliding(testPlayer->GetComponent<BoxCollider>("BoxCollider").get()))
	{
		dynamic_pointer_cast<DialogTextBox>(text[1])->Activate();
		if (INPUT->KeyDown(VK_RETURN))
		{
			SCENE->LoadScene("Stage01");
		}
	}

	if (log[0]->GetComponent<BoxCollider>("BoxCollider")->IsColliding(lumberAxe->GetComponent<BoxCollider>("BoxCollider").get()) && isTest == false)
	{
		isTest = true;
		if (isTest == true)
		{
			log[0]->GetComponent<LogPathCreator>("LogPathCreator")->OnLogCollision(lumberAxe->GetComponent<BoxCollider>("BoxCollider").get());
		}
	}

	if (lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->GetState() == AxeState::AXE_IDLE_L || lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->GetState() == AxeState::AXE_IDLE_R)
	{
		isTest = false;
	}

	if (log[0]->GetComponent<LogPathCreator>("LogPathCreator")->GetTransformed() == true && isTest2 == false)
	{
		log[0]->AddComponent(make_shared<FloorCollision>("FloorCollision", testPlayer, lumberAxe));
		isTest2 = true;
	}
}

void Scene_03::LateUpdate() {}

void Scene_03::Render()
{
	if (backgroundObject != nullptr) backgroundObject->Render();

	if (tmap != nullptr) tmap->Render();

	for (const auto& floors : floor) if (floors != nullptr) floors->Render();

	if (house[1] != nullptr) house[1]->Render();

	if (house[2] != nullptr) house[2]->Render();

	if (house[3] != nullptr) house[3]->Render();
	
	if (house[4] != nullptr) house[4]->Render();

	if (house[5] != nullptr) house[5]->Render();

	for (const auto& logs : log) if (logs != nullptr) logs->Render();

	if (lumberAxe != nullptr) lumberAxe->Render();

	if (testPlayer != nullptr) testPlayer->Render();

	if (house[0] != nullptr) house[0]->Render();

	for (const auto& textPoint : textPoint) if (textPoint != nullptr) textPoint->Render();

	for (const auto& text : text) if (text != nullptr) text->Render();
}

void Scene_03::PostRender()
{
	if (text[0] != nullptr) dynamic_pointer_cast<DialogTextBox>(text[0])->PostRender();
	if (text[1] != nullptr) dynamic_pointer_cast<DialogTextBox>(text[1])->PostRender();
}

void Scene_03::ImGUI()
{

}


