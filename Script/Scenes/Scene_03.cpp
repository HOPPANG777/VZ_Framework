#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake()
{
	isLoaded = false;

	// Player
	{
		testPlayer = std::make_shared<PlayerUnit>(CENTER + Vector3(-550, -310, 0));
		lumberAxe = std::make_shared<LumberAxe>(CENTER + Vector3(-550, -200, 0));
		lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->SetTarget(testPlayer.get());
	}
	
	// Floor
	{
		const auto& floorLine1 = std::make_shared<ColorLine>(CENTER + Vector3(5800, -320, 0), 0, 0.0f, CLEAR);
		floorLine1->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10000, 10, 0), GREEN));
		floor.push_back(floorLine1);

		const auto& floorLine2 = std::make_shared<ColorLine>(CENTER + Vector3(-170, -320, 0), 0, 0.0f, CLEAR);
		floorLine2->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(950, 10, 0), GREEN));
		floor.push_back(floorLine2);
	}

	// Floor Collider
	{
		for (const auto& flr : floor) if (flr != nullptr) flr->AddComponent(std::make_shared<FloorCollision>("FloorCollision"));
		for (const auto& flr : floor) if (flr != nullptr) flr->GetComponent<FloorCollision>("FloorCollision")->SetPlayer(testPlayer.get());
		for (const auto& flr : floor) if (flr != nullptr) flr->GetComponent<FloorCollision>("FloorCollision")->SetLumberAxe(lumberAxe.get());
		for (const auto& flr : floor) if (flr != nullptr) COLLISION->AddCollider(flr->GetComponent<BoxCollider>("BoxCollider"));
	}
	
	// House
	{
		house.push_back(std::make_shared<TextureRect>(CENTER + Vector3(-450, -180, 0), Vector3(300, 300, 0), 0.0f, TexturePath + L"Object/House01.png"));
		house.push_back(std::make_shared<TextureRect>(CENTER + Vector3(-450, -180, 0), Vector3(300, 300, 0), 0.0f, TexturePath + L"Object/House02.png"));
		house.push_back(std::make_shared<TextureRect>(CENTER + Vector3(-450, -180, 0), Vector3(300, 300, 0), 0.0f, TexturePath + L"Object/House03.png"));
		house.push_back(std::make_shared<TextureRect>(CENTER + Vector3(-300, -280, 0), Vector3(70, 70, 0), 0.0f, TexturePath + L"Object/Wood.png"));
		house.push_back(std::make_shared<TextureRect>(CENTER + Vector3(1300, -190, 0), Vector3(250, 250, 0), 0.0f, TexturePath + L"Object/ObjectHouse01.png"));
		house.push_back(std::make_shared<TextureRect>(CENTER + Vector3(2100, -190, 0), Vector3(250, 250, 0), 0.0f, TexturePath + L"Object/ObjectHouse02.png"));
	}

	// Text
	{
		text.push_back(std::make_shared<DialogTextBox>("Text_01", CENTER + Vector3(0, -300, 0), CENTER + Vector3(0, -300, 0), Vector3(640, 200, 0), 0.0f, TexturePath + L"UI/Panel1.png", TexturePath + L"UI/Arrow.png", L"제100회팔씨름챔피언선발토너먼트 선수 등록 마감 까지 일주일... 선수 등록을 진행하는 도시까지 전속력으로 달린다!", 25.0f, L"메이플스토리 OTF", BLACK));
		textPoint.push_back(std::make_shared<Object>("TextPoint_01", CENTER + Vector3(-300, -300, 0), Vector3(Values::ZeroVec3), 0.0f));
		text.push_back(std::make_shared<DialogTextBox>("Text_02", CENTER + Vector3(0, -300, 0), CENTER + Vector3(1920, -300, 0), Vector3(640, 200, 0), 0.0f, TexturePath + L"UI/Panel1.png", TexturePath + L"UI/Arrow.png", L"이제 이곳을 떠날 때가 되었나? (Enter를 입력해주세요.)", 25.0f, L"메이플스토리 OTF", BLACK));
		textPoint.push_back(std::make_shared<Object>("TextPoint_02", CENTER + Vector3(2400, -330, 0), Vector3(Values::ZeroVec3), 0.0f));
	}

	// TextPoint Collider
	{
		for (const auto& textP : textPoint) textP->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 100, 0), BLACK));
		for (const auto& textP : textPoint) COLLISION->AddCollider(textP->GetComponent<BoxCollider>("BoxCollider"));
	}

	// Background
	{
		const auto& parallax = std::make_shared<ParallaxBackground>("Parallax", 5);

		for (int i = 0; i < 5; i++)
		{
			bgLayer[i] = std::make_shared<TextureRect>(CENTER + Vector3(0, 0, 0), Vector3(WIN_DEFAULT_WIDTH * 1.2f, WIN_DEFAULT_HEIGHT * 1.2f, 0), 0.0f, TexturePath + L"Background/Stage01/" + std::to_wstring(i + 1) + L".png");
			bgLayerDup[i] = std::make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH * 1.2f, 0, 0), Vector3(WIN_DEFAULT_WIDTH * 1.2f, WIN_DEFAULT_HEIGHT * 1.2f, 0), 0.0f, TexturePath + L"Background/Stage01/" + std::to_wstring(i + 1) + L".png");

			bgOrigin[i] = bgLayer[i]->GetPosition();
			bgOriginDup[i] = bgLayerDup[i]->GetPosition();

			parallax->SetLayer(i, bgLayer[i].get(), bgLayerDup[i].get());
		}

		backgroundObject = std::make_shared<Object>("Background");
		backgroundObject->AddComponent(parallax);
	}

	// TileMap
	{
		UINT spacing = 50;
		UINT width = (WIN_DEFAULT_WIDTH * 2) / spacing;
		UINT height = (WIN_DEFAULT_HEIGHT * 2) / spacing;

		tmap = std::make_unique<TileMap>(width, height, spacing);
		tmap->Load("_Data/_TileData/map01.data");
	}
	
	// Log
	{
		const auto& logRect = std::make_shared<TextureRect>(CENTER + Vector3(260, -65, 0), Vector3(300, 500, 0), 0.0f, TexturePath + L"Object/Log.png");
		logRect->AddComponent(std::make_shared<HitComponent>("HitComponent"));
		logRect->AddComponent(std::make_shared<LogPathCreator>());
		logRect->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 500, 0), BLACK));
		COLLISION->AddCollider(logRect->GetComponent<BoxCollider>("BoxCollider"));
		log.push_back(logRect);
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
	if (tmap != nullptr) tmap.reset();
	for (auto& floors : floor)if (floors != nullptr) floors.reset();
	for (auto& houseElem : house)if (houseElem != nullptr) houseElem.reset();
	for (auto& bg : bgLayer)if (bg != nullptr) bg.reset();
	for (auto& bgdup : bgLayerDup)if (bgdup != nullptr) bgdup.reset();
	for (auto& t : text)if (t != nullptr) t.reset();
	for (auto& tp : textPoint)if (tp != nullptr) tp.reset();
	for (auto& l : log) if (l != nullptr) l.reset();
	if (testPlayer != nullptr) testPlayer.reset();
	if (lumberAxe != nullptr) lumberAxe.reset();
	if (backgroundObject != nullptr) backgroundObject.reset();
	COLLISION->RemoveAllCollider();
}

void Scene_03::FixedUpdate() {}

void Scene_03::Update()
{
	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Update();
	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();
	for (const auto& floors : floor) if (floors != nullptr) floors->Update();
	for (const auto& houseElem : house) if (houseElem != nullptr) houseElem->Update();
	for (const auto& t : text) if (t != nullptr) t->Update();
	for (const auto& tp : textPoint) if (tp != nullptr) tp->Update();
	for (const auto& l : log) if (l != nullptr) l->Update();
	if (tmap != nullptr) tmap->Update();
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
		if (lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->GetState() == AxeState::AXE_IDLE_L || lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->GetState() == AxeState::AXE_IDLE_R) return;
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
		log[0]->AddComponent(make_shared<FloorCollision>("FloorCollision"));
		log[0]->GetComponent<FloorCollision>("FloorCollision")->SetPlayer(testPlayer.get());
		log[0]->GetComponent<FloorCollision>("FloorCollision")->SetLumberAxe(lumberAxe.get());
		isTest2 = true;
	}
}

void Scene_03::LateUpdate() {}

void Scene_03::Render()
{
	if (backgroundObject != nullptr) backgroundObject->Render();
	if (tmap != nullptr) tmap->Render();
	for (const auto& floors : floor) if (floors != nullptr) floors->Render();
	for (const auto& l : log) if (l != nullptr) l->Render();
	if (house[1] != nullptr) house[1]->Render();
	if (house[2] != nullptr) house[2]->Render();
	if (house[3] != nullptr) house[3]->Render();
	if (house[4] != nullptr) house[4]->Render();
	if (house[5] != nullptr) house[5]->Render();
	if (lumberAxe != nullptr) lumberAxe->Render();
	if (testPlayer != nullptr) testPlayer->Render();
	if (house[0] != nullptr) house[0]->Render();
	for (const auto& tp : textPoint) if (tp != nullptr) tp->Render();
	for (const auto& t : text) if (t != nullptr) t->Render();
}

void Scene_03::PostRender()
{
	if (text[0] != nullptr) dynamic_pointer_cast<DialogTextBox>(text[0])->PostRender();
	if (text[1] != nullptr) dynamic_pointer_cast<DialogTextBox>(text[1])->PostRender();
}

void Scene_03::ImGUI()
{

}


