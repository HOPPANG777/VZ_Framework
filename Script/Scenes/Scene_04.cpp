#include "stdafx.h"
#include "Scene_04.h"

void Scene_04::Awake()
{
	isLoaded = false;

	// Player
	{
		testPlayer = std::make_shared<PlayerUnit>(Values::GetCenter() + Vector3(-550, -45, 0));
		lumberAxe = std::make_shared<LumberAxe>(Values::GetCenter() + Vector3(-550, -30, 0));
		lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->SetTarget(testPlayer.get());
		SceneManager::Get()->SetPlayer(testPlayer);
	}

	// Floor
	{
		floor.push_back(std::make_shared<ColorLine>(Values::GetCenter() + Vector3(-550, -60, 0), 0, 0.0f, CLEAR));
		floor[0]->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10000, 1, 0), GREEN));
	}

	// Floor Collider
	{
		for (const auto& flr : floor) if (flr != nullptr) flr->AddComponent(std::make_shared<FloorCollision>("FloorCollision"));
		for (const auto& flr : floor) if (flr != nullptr) flr->GetComponent<FloorCollision>("FloorCollision")->SetPlayer(testPlayer.get());
		for (const auto& flr : floor) if (flr != nullptr) flr->GetComponent<FloorCollision>("FloorCollision")->SetLumberAxe(lumberAxe.get());
		for (const auto& flr : floor) if (flr != nullptr) CollisionManager::Get()->AddCollider(flr->GetComponent<BoxCollider>("BoxCollider"));
	}

	// Background
	{
		const auto& parallax = std::make_shared<ParallaxBackground>("Parallax", 5);

		// 배경 레이어 2개씩 생성
		for (int i = 0; i < 5; i++)
		{
			bgLayer[i] = std::make_shared<TextureRect>(Values::GetCenter() + Vector3(0, 0, 0), Vector3(Values::GetWinWidth() * 1.2f, Values::GetWinHeight() * 1.2f, 0), 0.0f, TexturePath + L"Background/Stage01/" + std::to_wstring(i + 1) + L".png");
			bgLayerDup[i] = std::make_shared<TextureRect>(Values::GetCenter() + Vector3(Values::GetWinWidth() * 1.2f, 0, 0), Vector3(Values::GetWinWidth() * 1.2f, Values::GetWinHeight() * 1.2f, 0), 0.0f, TexturePath + L"Background/Stage01/" + std::to_wstring(i + 1) + L".png");

			bgOrigin[i] = bgLayer[i]->GetPosition();
			bgOriginDup[i] = bgLayerDup[i]->GetPosition();

			parallax->SetLayer(i, bgLayer[i].get(), bgLayerDup[i].get());
		}

		backgroundObject = std::make_shared<GameObject>("Background");
		backgroundObject->AddComponent(parallax);
	}

	// TileMap
	{
		float spacing = 50;
		float width = (Values::GetWinWidth() * 2) / spacing;
		float height = (Values::GetWinHeight() * 2) / spacing;

		tmap = std::make_unique<TileMap>(width, height, spacing);
		tmap->Load("_Data/_TileData/map02.data");
	}

	// Puzzle Rock
	{
		puzzleRock = std::make_shared<TextureRect>(Values::GetCenter() + Vector3(-50, 45, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"GameObject/PuzzleRock.png");
		puzzleRock->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(200, 200, 0), BLACK));
		CollisionManager::Get()->AddCollider(puzzleRock->GetComponent<BoxCollider>("BoxCollider"));
		puzzleRock->AddComponent(std::make_shared<PushableComponent>());
		puzzleRock->SetName("PuzzleRock");
	}

	// bridge
	{
		bridge = std::make_shared<TextureRect>(Values::GetCenter() + Vector3(360, 0, 0), Vector3(600, 200, 0), 0.0f, TexturePath + L"GameObject/ObjectBridge.png");
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
		
		windObject = std::make_shared<Wind>(Values::GetCenter() + Vector3(360, 0, 0), Vector3(600, 600, 0), 0.0f, TexturePath + L"GameObject/Wind.png");
		windObject->AddComponent(std::make_shared<WindZone>("WindZone", Values::GetCenter() + windRegionMin, Values::GetCenter() + windRegionMax, windDirection, windIntensity));
	}

	// Text
	{
		text.push_back(std::make_shared<DialogTextBox>("Text_01", Values::GetCenter() + Vector3(0, -300, 0), Values::GetCenter() + Vector3(0, -300, 0), Vector3(640, 200, 0), 0.0f, TexturePath + L"UI/Panel1.png", TexturePath + L"UI/Arrow.png", L"저기 엄청난 바람이 불어오는군, 저 바위를 들고가면 밀려나지 않을 수 있겠어.", 25.0f, L"메이플스토리 OTF", BLACK));
		textPoint.push_back(std::make_shared<GameObject>("TextPoint_01", Values::GetCenter() + Vector3(-300, 45, 0), Vector3(Values::ZeroVec3), 0.0f));
		text.push_back(std::make_shared<DialogTextBox>("Text_02", Values::GetCenter() + Vector3(0, -300, 0), Values::GetCenter() + Vector3(1280, -300, 0), Vector3(640, 200, 0), 0.0f, TexturePath + L"UI/Panel1.png", TexturePath + L"UI/Arrow.png", L"이제 다음으로 가자. (Enter를 입력해주세요.)", 25.0f, L"메이플스토리 OTF", BLACK));
		textPoint.push_back(std::make_shared<GameObject>("TextPoint_02", Values::GetCenter() + Vector3(1380, 45, 0), Vector3(Values::ZeroVec3), 0.0f));
	}

	// TextPoint Collider
	{
		for (const auto& textP : textPoint) textP->AddComponent(std::make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(100, 100, 0), BLACK));
		for (const auto& textP : textPoint) CollisionManager::Get()->AddCollider(textP->GetComponent<BoxCollider>("BoxCollider"));
	}

	// Sound
	{
		Sounds::Get()->AddSound("Wind", SoundPath + L"_NordicLumberJack/Wind.wav", true);
		Sounds::Get()->SetVolume("Wind", 0.2f);
	}

	isLoaded = true;
}

void Scene_04::Start()
{
	lumberAxe->GetComponent<LumberAxeComponent>("LumberAxeComponent")->Recall();
	std::dynamic_pointer_cast<DialogTextBox>(text[0])->Deactivate();
	std::dynamic_pointer_cast<DialogTextBox>(text[1])->Deactivate();
	Sounds::Get()->Play("Wind");
}

void Scene_04::Destroy()
{
	if (tmap != nullptr) tmap.reset();
	if (backgroundObject != nullptr) backgroundObject.reset();
	if (puzzleRock != nullptr) puzzleRock.reset();
	if (lumberAxe != nullptr) lumberAxe.reset();
	if (testPlayer != nullptr) testPlayer.reset();
	if (windObject != nullptr) windObject.reset();
	if (bridge != nullptr) bridge.reset();
	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();
	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup.reset();
	for (auto& floors : floor) if (floors != nullptr) floors.reset();
	for (auto& t : text) if (t != nullptr) t.reset();
	for (auto& tp : textPoint) if (tp != nullptr) tp.reset();
	SceneManager::Get()->ResetPlayer();
	CollisionManager::Get()->RemoveAllCollider();
	Sounds::Get()->Stop("Wind");
	Sounds::Get()->RemoveSound("Wind");
}

void Scene_04::FixedUpdate()
{
}

void Scene_04::Update()
{
	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Update();
	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();
	for (const auto& floors : floor) if (floors != nullptr) floors->Update();
	for (const auto& t : text) if (t != nullptr) t->Update();
	for (const auto& tp : textPoint) if (tp != nullptr) tp->Update();
	if (tmap != nullptr) tmap->Update();
	if (backgroundObject != nullptr) backgroundObject->Update();
	if (puzzleRock != nullptr) puzzleRock->Update();
	if (lumberAxe != nullptr) lumberAxe->Update();
	if (testPlayer != nullptr) testPlayer->Update();
	if (windObject != nullptr) windObject->Update();
	if (bridge != nullptr) bridge->Update();

	Camera::Get()->UpdateHorizontalScroll(testPlayer.get(), 0.5f, 1.5f);

	if (textPoint[0]->GetComponent<BoxCollider>("BoxCollider")->IsColliding(testPlayer->GetComponent<BoxCollider>("BoxCollider").get()) && text[0]->GetComponent<TypingText>("TypingText")->GetTextOut() == false)
	{
		std::dynamic_pointer_cast<DialogTextBox>(text[0])->Activate();
	}

	if (textPoint[1]->GetComponent<BoxCollider>("BoxCollider")->IsColliding(testPlayer->GetComponent<BoxCollider>("BoxCollider").get()))
	{
		std::dynamic_pointer_cast<DialogTextBox>(text[1])->Activate();
		if (Input::Get()->KeyDown(VK_RETURN))
		{
			SceneManager::Get()->LoadScene("Stage02");
		}
	}
}

void Scene_04::LateUpdate()
{
}

void Scene_04::Render()
{
	if (backgroundObject != nullptr) backgroundObject->Render();
	if (tmap != nullptr) tmap->Render();
	for (const auto& floors : floor) if (floors != nullptr) floors->Render();
	if (bridge != nullptr) bridge->Render();
	if (puzzleRock != nullptr) puzzleRock->Render();
	if (lumberAxe != nullptr) lumberAxe->Render();
	if (testPlayer != nullptr) testPlayer->Render();
	if (windObject != nullptr) windObject->Render();
	for (const auto& t : text) if (t != nullptr) t->Render();
	for (const auto& tp : textPoint) if (tp != nullptr) tp->Render();
}

void Scene_04::PostRender()
{
	if (text[0] != nullptr) std::dynamic_pointer_cast<DialogTextBox>(text[0])->PostRender();
	if (text[1] != nullptr) std::dynamic_pointer_cast<DialogTextBox>(text[1])->PostRender();
}

void Scene_04::ImGUI()
{
}
