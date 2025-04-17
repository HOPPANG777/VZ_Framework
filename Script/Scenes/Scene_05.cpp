#include "stdafx.h"
#include "Scene_05.h"

void Scene_05::Awake()
{
	isLoaded = false;
	
	// Player
	{
		testPlayer = std::make_shared<PlayerUnit>(CENTER + Vector3(-550, -320, 0));
		lumberAxe = std::make_shared<LumberAxe>(CENTER + Vector3(-550, -320, 0));
		lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->SetTarget(testPlayer.get());
		SCENE->SetPlayer(testPlayer);
	}

	// Floor
	{
		floor.push_back(std::make_shared<ColorLine>(CENTER + Vector3(-250, -320, 0), 0, 0.0f, CLEAR));
		floor[0]->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(750, 1, 0), GREEN));
		floor.push_back(std::make_shared<ColorLine>(CENTER + Vector3(1100, 230, 0), 0, 0.0f, CLEAR));
		floor[1]->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(2000, 1, 0), GREEN));
	}

	// Floor Collider
	{
		for (const auto& flr : floor) if (flr != nullptr) flr->AddComponent(std::make_shared<FloorCollision>("FloorCollision"));
		for (const auto& flr : floor) if (flr != nullptr) flr->GetComponent<FloorCollision>("FloorCollision")->SetPlayer(testPlayer.get());
		for (const auto& flr : floor) if (flr != nullptr) flr->GetComponent<FloorCollision>("FloorCollision")->SetLumberAxe(lumberAxe.get());
		for (const auto& flr : floor) if (flr != nullptr) COLLISION->AddCollider(flr->GetComponent<BoxCollider>("BoxCollider"));
	}

	// Wall
	{
		wall.push_back(std::make_shared<ColorLine>(CENTER + Vector3(120, -120, 0), 0, 0.0f, CLEAR));
		wall[0]->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10, 750, 0), GREEN));
	}

	// Wall Collider
	{
		//for (const auto& wal : wall) if (wal != nullptr) wal->AddComponent(std::make_shared<WallCollision>("WallCollision", false));
		wall[0]->AddComponent(std::make_shared<WallCollision>("WallCollision", false));
		for (const auto& wal : wall) if (wal != nullptr) wal->GetComponent<WallCollision>("WallCollision")->SetPlayer(testPlayer.get());
		for (const auto& wal : wall) if (wal != nullptr) wal->GetComponent<WallCollision>("WallCollision")->SetLumberAxe(lumberAxe.get());
		for (const auto& wal : wall) if (wal != nullptr) COLLISION->AddCollider(wal->GetComponent<BoxCollider>("BoxCollider"));
	}

	// Background
	{
		std::shared_ptr<ParallaxBackground> parallax = std::make_shared<ParallaxBackground>("Parallax", 5);

		// 배경 레이어 2개씩 생성
		for (int i = 0; i < 5; i++)
		{
			bgLayer[i] = std::make_shared<TextureRect>(CENTER + Vector3(0, 0, 0), Vector3(WIN_DEFAULT_WIDTH * 1.2f, WIN_DEFAULT_HEIGHT * 1.2f, 0), 0.0f, TexturePath + L"Background/Stage01/" + to_wstring(i + 1) + L".png");
			bgLayerDup[i] = std::make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH * 1.2f, 0, 0), Vector3(WIN_DEFAULT_WIDTH * 1.2f, WIN_DEFAULT_HEIGHT * 1.2f, 0), 0.0f, TexturePath + L"Background/Stage01/" + to_wstring(i + 1) + L".png");

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
		tmap->Load("_Data/_TileData/map03.data");
	}

	// Ladder
	{
		ladder = std::make_shared<TextureRect>(CENTER + Vector3(100, 230, 0), Vector3(100, 100, 0), 0.0f, TexturePath + L"Object/Ladder1.png");
		ladder->AddComponent(std::make_shared<LadderTransComponent>());
		ladder2 = std::make_shared<TextureRect>(CENTER + Vector3(100, 0, 0), Vector3(100, 520, 0), 0.0f, TexturePath + L"Object/Ladder2.png");
		ladder->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 100, 0), GREEN));
		COLLISION->AddCollider(ladder->GetComponent<BoxCollider>("BoxCollider"));
		//ladder2->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 520, 0), GREEN));
		//COLLISION->AddCollider(ladder2->GetComponent<BoxCollider>("BoxCollider"));
	}

	// Text
	{
		text.push_back(std::make_shared<DialogTextBox>("Text_01", CENTER + Vector3(0, -300, 0), CENTER + Vector3(0, -300, 0), Vector3(640, 200, 0), 0.0f, TexturePath + L"UI/Panel1.png", TexturePath + L"UI/Arrow.png", L"저 절벽은 너무 높아... 사다리를 내려야겠어.", 25.0f, L"마비옛체_OTF", BLACK));
		textPoint.push_back(std::make_shared<Object>("TextPoint_01", CENTER + Vector3(-300, -300, 0), Vector3(Values::ZeroVec3), 0.0f));
		text.push_back(std::make_shared<DialogTextBox>("Text_02", CENTER + Vector3(0, -300, 0), CENTER + Vector3(1280, 300, 0), Vector3(640, 200, 0), 0.0f, TexturePath + L"UI/Panel1.png", TexturePath + L"UI/Arrow.png", L"이제 다음으로 가자. (Enter를 입력해주세요.)", 25.0f, L"마비옛체_OTF", BLACK));
		textPoint.push_back(std::make_shared<Object>("TextPoint_02", CENTER + Vector3(1380, 300, 0), Vector3(Values::ZeroVec3), 0.0f));
	}

	// TextPoint Collider
	{
		for (const auto& textP : textPoint) textP->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 100, 0), BLACK));
		for (const auto& textP : textPoint) COLLISION->AddCollider(textP->GetComponent<BoxCollider>("BoxCollider"));
	}


	isLoaded = true;
}

void Scene_05::Start()
{
	lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->Recall();
	dynamic_pointer_cast<DialogTextBox>(text[0])->Deactivate();
	dynamic_pointer_cast<DialogTextBox>(text[1])->Deactivate();
	dynamic_pointer_cast<TextureRect>(ladder2)->SetColor(CLEAR);
}

void Scene_05::Destroy()
{
	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();
	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup.reset();
	for (auto& flr : floor) if (flr != nullptr) flr.reset();
	for (auto& t : text) if (t != nullptr) t.reset();
	for (auto& tp : textPoint) if (tp != nullptr) tp.reset();
	for (auto& wal : wall) if (wal != nullptr) wal.reset();
	if (backgroundObject != nullptr) backgroundObject.reset();
	if (testPlayer != nullptr) testPlayer.reset();
	if (lumberAxe != nullptr) lumberAxe.reset();
	if (ladder != nullptr) ladder.reset();
	if (ladder2 != nullptr) ladder2.reset();
	if (tmap != nullptr) tmap.reset();
	COLLISION->RemoveAllCollider();
	SCENE->ResetPlayer();
}

void Scene_05::FixedUpdate()
{
}

void Scene_05::Update()
{
	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Update();
	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();
	for (const auto& flr : floor) if (flr != nullptr) flr->Update();
	for (const auto& wal : wall) if (wal != nullptr) wal->Update();
	for (const auto& t : text) if (t != nullptr) t->Update();
	for (const auto& tp : textPoint) if (tp != nullptr) tp->Update();
	if (backgroundObject != nullptr) backgroundObject->Update();
	if (tmap != nullptr) tmap->Update();
	if (ladder != nullptr) ladder->Update();
	if (ladder2 != nullptr) ladder2->Update();
	if (lumberAxe != nullptr) lumberAxe->Update();
	if (testPlayer != nullptr) testPlayer->Update();

	CAMERA->UpdateHorizontalScroll(testPlayer.get(), 0.5f, 1.5f);

	if (textPoint[0]->GetComponent<BoxCollider>("BoxCollider")->IsColliding(testPlayer->GetComponent<BoxCollider>("BoxCollider").get()) && text[0]->GetComponent<TypingText>("TypingText")->GetTextOut() == false)
	{
		dynamic_pointer_cast<DialogTextBox>(text[0])->Activate();
	}

	if (textPoint[1]->GetComponent<BoxCollider>("BoxCollider")->IsColliding(testPlayer->GetComponent<BoxCollider>("BoxCollider").get()))
	{
		dynamic_pointer_cast<DialogTextBox>(text[1])->Activate();
		if (INPUT->KeyDown(VK_RETURN))
		{
			SCENE->LoadScene("Stage03");
		}
	}

	if (ladder->GetComponent<BoxCollider>("BoxCollider")->IsColliding(lumberAxe->GetComponent<BoxCollider>("BoxCollider").get()) && isTest == false)
	{
		isTest = true;
		if (isTest == true)
		{
			ladder->GetComponent<LadderTransComponent>("LadderTransComponent")->OnLadderCollision(lumberAxe->GetComponent<BoxCollider>("BoxCollider").get());
		}
	}

	if (lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->GetState() == AxeState::AXE_IDLE_L || lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->GetState() == AxeState::AXE_IDLE_R)
	{
		isTest = false;
	}

	if (ladder->GetComponent<LadderTransComponent>("LadderTransComponent")->GetTransformed() == true && isTest2 == false)
	{
		dynamic_pointer_cast<TextureRect>(ladder)->SetColor(CLEAR);
		dynamic_pointer_cast<TextureRect>(ladder2)->SetColor(WHITE);
		ladder->AddComponent(std::make_shared<LadderComponent>());
		isTest2 = true;
	}

}

void Scene_05::LateUpdate()
{
}

void Scene_05::Render()
{
	if (backgroundObject != nullptr) backgroundObject->Render();
	if (tmap != nullptr) tmap->Render();
	for (const auto& flr : floor) if (flr != nullptr) flr->Render();
	for (const auto& wal : wall) if (wal != nullptr) wal->Render();
	if (ladder != nullptr) ladder->Render();
	if (ladder2 != nullptr) ladder2->Render();
	if (lumberAxe != nullptr) lumberAxe->Render();
	if (testPlayer != nullptr) testPlayer->Render();
	for (const auto& t : text) if (t != nullptr) t->Render();
	for (const auto& tp : textPoint) if (tp != nullptr) tp->Render();
}

void Scene_05::PostRender()
{
	if (text[0] != nullptr) dynamic_pointer_cast<DialogTextBox>(text[0])->PostRender();
	if (text[1] != nullptr) dynamic_pointer_cast<DialogTextBox>(text[1])->PostRender();
}

void Scene_05::ImGUI()
{
}
