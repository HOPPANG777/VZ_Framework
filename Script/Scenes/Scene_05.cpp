#include "stdafx.h"
#include "Scene_05.h"

void Scene_05::Awake()
{
	isLoaded = false;
	
	// Player
	{
		testPlayer = std::make_shared<PlayerUnit>(Values::GetCenter() + Vector3(-550, -300, 0));
		lumberAxe = std::make_shared<LumberAxe>(Values::GetCenter() + Vector3(-550, -310, 0));
		lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->SetTarget(testPlayer.get());
		SCENE->SetPlayer(testPlayer);
                heartUIBase = Vector3(-Values::GetWinWidth() * 0.45f, Values::GetWinHeight() * 0.45f, 0);
		                Vector3 heartPos = Values::GetCenter() + heartUIBase;
		for (int i = 0; i < 5; i++)
		{
			heartUI.push_back(std::make_shared<TextureRect>(heartPos + Vector3(i * 40, 0, 0), Vector3(30, 30, 0), 0.0f, TexturePath + L"UI/FullHearts.png", TransformType::Rect));
		}
	}

	// Floor
	{
		floor.push_back(std::make_shared<ColorLine>(Values::GetCenter() + Vector3(-250, -320, 0), 0, 0.0f, CLEAR));
		floor[0]->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(750, 5, 0), GREEN));
		floor.push_back(std::make_shared<ColorLine>(Values::GetCenter() + Vector3(1100, 230, 0), 0, 0.0f, CLEAR));
		floor[1]->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(2000, 5, 0), GREEN));
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
		wall.push_back(std::make_shared<ColorLine>(Values::GetCenter() + Vector3(120, -150, 0), 0, 0.0f, CLEAR));
		wall[0]->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10, 700, 0), GREEN));
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
			bgLayer[i] = std::make_shared<TextureRect>(Values::GetCenter() + Vector3(0, 0, 0), Vector3(Values::GetWinWidth() * 1.2f, Values::GetWinHeight() * 1.2f, 0), 0.0f, TexturePath + L"Background/Stage01/" + to_wstring(i + 1) + L".png");
			bgLayerDup[i] = std::make_shared<TextureRect>(Values::GetCenter() + Vector3(Values::GetWinWidth() * 1.2f, 0, 0), Vector3(Values::GetWinWidth() * 1.2f, Values::GetWinHeight() * 1.2f, 0), 0.0f, TexturePath + L"Background/Stage01/" + to_wstring(i + 1) + L".png");

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
		UINT width = (Values::GetWinWidth() * 2) / spacing;
		UINT height = (Values::GetWinHeight() * 2) / spacing;

		tmap = std::make_unique<TileMap>(width, height, spacing);
		tmap->Load("_Data/_TileData/map03.data");
	}

	// Ladder
	{
		ladder = std::make_shared<TextureRect>(Values::GetCenter() + Vector3(100, 230, 0), Vector3(100, 100, 0), 0.0f, TexturePath + L"Object/Ladder1.png");
		ladder->AddComponent(std::make_shared<LadderTransComponent>());
		ladder->AddComponent(std::make_shared<HitComponent>());
		ladder2 = std::make_shared<TextureRect>(Values::GetCenter() + Vector3(100, 0, 0), Vector3(100, 550, 0), 0.0f, TexturePath + L"Object/Ladder2.png");
		ladder->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 100, 0), GREEN));
		COLLISION->AddCollider(ladder->GetComponent<BoxCollider>("BoxCollider"));
	}

	// Text
	{
		text.push_back(std::make_shared<DialogTextBox>("Text_01", Values::GetCenter() + Vector3(0, -300, 0), Values::GetCenter() + Vector3(0, -300, 0), Vector3(640, 200, 0), 0.0f, TexturePath + L"UI/Panel1.png", TexturePath + L"UI/Arrow.png", L"저 절벽은 너무 높아... 사다리를 내려야겠어.", 25.0f, L"메이플스토리 OTF", BLACK));
		textPoint.push_back(std::make_shared<Object>("TextPoint_01", Values::GetCenter() + Vector3(-320, -300, 0), Vector3(Values::ZeroVec3), 0.0f));
		text.push_back(std::make_shared<DialogTextBox>("Text_02", Values::GetCenter() + Vector3(0, -300, 0), Values::GetCenter() + Vector3(770, -10, 0), Vector3(640, 200, 0), 0.0f, TexturePath + L"UI/Panel1.png", TexturePath + L"UI/Arrow.png", L"감히 내 앞길을 가로막다니! 죽인다!!! (Enter를 입력해주세요, 전투 씬으로 넘어갑니다.)", 25.0f, L"메이플스토리 OTF", BLACK));
		textPoint.push_back(std::make_shared<Object>("TextPoint_02", Values::GetCenter() + Vector3(840, 300, 0), Vector3(Values::ZeroVec3), 0.0f));
	}

	// TextPoint Collider
	{
		for (const auto& textP : textPoint) textP->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 100, 0), BLACK));
		for (const auto& textP : textPoint) COLLISION->AddCollider(textP->GetComponent<BoxCollider>("BoxCollider"));
	}

	{
		bback = std::make_shared<TextureRect>(Values::GetCenter() + Vector3(1200, 270, 0), Vector3(100, 100, 0), 0.0f, TexturePath + L"Animation/Bback/Bback.png");
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
	for (auto& h : heartUI) if (h != nullptr) h.reset();
	if (backgroundObject != nullptr) backgroundObject.reset();
	if (testPlayer != nullptr) testPlayer.reset();
	if (bback != nullptr) bback.reset();
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
	if (tmap != nullptr) tmap->Update();
	if (backgroundObject != nullptr) backgroundObject->Update();
	if (ladder != nullptr) ladder->Update();
	if (ladder2 != nullptr) ladder2->Update();
	if (testPlayer != nullptr) testPlayer->Update();
	if (lumberAxe != nullptr) lumberAxe->Update();
	if (bback != nullptr) bback->Update();
	for (const auto& h : heartUI) if (h != nullptr) h->Update();

	CAMERA->UpdateHorizontalScroll(testPlayer.get(), 0.5f, 1.1f);
	CAMERA->UpdateVerticalScroll(testPlayer.get(), 0.5f, 1.5f);

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

	AxeState axeState = lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->GetState();

	if (ladder->GetComponent<BoxCollider>("BoxCollider")->IsColliding(lumberAxe->GetComponent<BoxCollider>("BoxCollider").get()) && isTest == false)
	{
		if (axeState == AxeState::AXE_IDLE_L || axeState == AxeState::AXE_IDLE_R) return;
		isTest = true;
		if (isTest == true)
		{
			ladder->GetComponent<LadderTransComponent>("LadderTransComponent")->OnLadderCollision(lumberAxe->GetComponent<BoxCollider>("BoxCollider").get());
		}
	}

	if (axeState == AxeState::AXE_IDLE_L || axeState == AxeState::AXE_IDLE_R)
	{
		isTest = false;
	}

	if (ladder->GetComponent<LadderTransComponent>("LadderTransComponent")->GetTransformed() == true && isTest2 == false)
	{
		isTest2 = true;
		if (isTest2 == true)
		{
			dynamic_pointer_cast<TextureRect>(ladder)->SetColor(CLEAR);
			dynamic_pointer_cast<TextureRect>(ladder2)->SetColor(WHITE);
			ladder->AddComponent(std::make_shared<LadderComponent>());
		}		
	}


        Vector3 basePos = CAMERA->GetPosition() + Values::GetCenter() + heartUIBase;
        for (int i = 0; i < heartUI.size(); i++)
                heartUI[i]->SetPosition(basePos + Vector3(i * 40, 0, 0));
	int hp = dynamic_pointer_cast<PlayerUnit>(testPlayer)->GetHealth();
	for (int i = 0; i < heartUI.size(); i++)
	{
		auto tex = dynamic_pointer_cast<TextureRect>(heartUI[i]);
		if (i < hp) tex->SetImage(TexturePath + L"UI/FullHearts.png");
		else tex->SetImage(TexturePath + L"UI/EmptyHearts.png");
	}
        if (testPlayer->GetPosition().y < -100)
        {
                dynamic_pointer_cast<PlayerUnit>(testPlayer)->DecreaseHealth();
                Vector3 startPos = Values::GetCenter() + Vector3(-550, -310, 0);
                testPlayer->SetPosition(startPos);
                if (lumberAxe != nullptr) lumberAxe->SetPosition(startPos + Vector3(0, 110, 0));

                auto controller = testPlayer->GetComponent<PlayerController>("PlayerController");
                if (controller) controller->Land();

                CAMERA->SetPosition(startPos - Values::GetCenter());
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
	if (ladder->GetComponent<LadderTransComponent>("LadderTransComponent")->GetTransformed() == true)
	{
		dynamic_pointer_cast<TextureRect>(ladder)->SetColor(CLEAR);
	}
	if (ladder != nullptr) ladder->Render();
	if (ladder2 != nullptr) ladder2->Render();
	if (lumberAxe != nullptr) lumberAxe->Render();
	if (testPlayer != nullptr) testPlayer->Render();
	if (bback != nullptr) bback->Render();
	for (const auto& t : text) if (t != nullptr) t->Render();
	for (const auto& tp : textPoint) if (tp != nullptr) tp->Render();
	for (const auto& h : heartUI) if (h != nullptr) h->Render();
}

void Scene_05::PostRender()
{
	if (text[0] != nullptr) dynamic_pointer_cast<DialogTextBox>(text[0])->PostRender();
	if (text[1] != nullptr) dynamic_pointer_cast<DialogTextBox>(text[1])->PostRender();
}

void Scene_05::ImGUI()
{
}
