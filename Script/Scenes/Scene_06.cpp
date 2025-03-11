#include "stdafx.h"
#include "Scene_06.h"

void Scene_06::Awake()
{
	isLoaded = false;

	objects.push_back(make_shared<ColorLine>(CENTER + Vector3(-640, -330, 0), 5000, 0.0f, CLEAR));

	testPlayer = make_shared<TextureRect>(CENTER + Vector3(-540, -300, 0), Vector3(118.5, 312, 0), 0.0f, TexturePath + L"Mirko1.png");

	// 배경 레이어 2개씩 생성
	for (int i = 0; i < 5; i++)
	{
		bgLayer[i] = make_shared<TextureRect>(CENTER + Vector3(0, 50, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/" + to_wstring(i + 1) + L".png");
		bgLayerDup[i] = make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH, 50, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/" + to_wstring(i + 1) + L".png");

		bgOrigin[i] = bgLayer[i]->GetPosition();
		bgOriginDup[i] = bgLayerDup[i]->GetPosition();
	}

	objects[0]->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(5000, 1, 0), GREEN));
	testPlayer->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(118.5, 312, 0), BLUE));
	testPlayer->AddComponent(make_shared<MoveComponent>(true));

	//TileMap
	{
		UINT spacing = 28;
		UINT width = (WIN_DEFAULT_WIDTH * 2) / spacing;
		UINT height = (WIN_DEFAULT_HEIGHT * 2) / spacing;

		tmap = make_unique<TileMap>(spacing, width, height);
		tmap->Load("_Data/_TileData/map04.data");
	}

	auto object01Collider = objects[0]->GetComponent<BoxCollider>("BoxCollider");
	auto testPlayerCollider = testPlayer->GetComponent<BoxCollider>("BoxCollider");

	if (object01Collider) COLLISION->AddCollider(object01Collider);
	if (testPlayerCollider) COLLISION->AddCollider(testPlayerCollider);

	isLoaded = true;
}

void Scene_06::Start()
{

}

void Scene_06::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();
	for (int i = 0; i < 5; i++) { if (bgLayer[i]) bgLayer[i].reset(); if (bgLayerDup[i]) bgLayerDup[i].reset(); }
	if (testPlayer != nullptr) testPlayer.reset();
}

void Scene_06::FixedUpdate() {}

void Scene_06::Update()
{
	if (tmap != nullptr) tmap->Update();

	for (int i = 0; i < 5; i++) { if (bgLayer[i]) bgLayer[i]->Update(); if (bgLayerDup[i]) bgLayerDup[i]->Update(); }

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();

	if (testPlayer)
	{
		// 중력 상수 (예: 980 픽셀/s², DELTA는 초 단위)
		const float gravityAcc = 980.0f;

		// testPlayer에 중력 적용 (아래로 가속)
		testPlayerVelocity.y -= gravityAcc * DELTA;

		// testPlayer를 이동 (이동은 Move() 함수가 내부에서 position을 더하는 방식)
		testPlayer->Move(testPlayerVelocity * DELTA);

		// floor와 testPlayer의 충돌 검사
		auto floorCollider = objects[0]->GetComponent<BoxCollider>("BoxCollider");
		auto playerCollider = testPlayer->GetComponent<BoxCollider>("BoxCollider");

		if (floorCollider && playerCollider)
		{
			// BoxCollider::IsColliding()를 통해 충돌 여부 판단
			if (floorCollider->IsColliding(playerCollider.get()))
			{
				// floor의 top (world 좌표 기준)
				float floorTop = floorCollider->GetCenter().y + (floorCollider->GetSize().y * 0.5f);
				// testPlayer의 bottom (world 좌표 기준)
				float playerBottom = playerCollider->GetCenter().y - (playerCollider->GetSize().y * 0.5f);

				// 만약 testPlayer가 floor 안으로 침투했다면, 보정값 계산
				float correction = floorTop - playerBottom;
				if (correction > 0)
				{
					// testPlayer의 위치를 보정하여 floor 위에 서도록 함
					testPlayer->GetComponent<Transform>("Transform")->SetPosition(testPlayer->GetPosition() + Vector3(0, correction, 0));
					// 충돌 시 수직 속도를 0으로 초기화
				}
			}

			if (testPlayer->GetPosition().x > WIN_DEFAULT_WIDTH * 0.5 && testPlayer->GetPosition().x < WIN_DEFAULT_WIDTH * 1.5)
			{
				Vector3 newCamPos = Vector3(testPlayer->GetPosition().x - WIN_DEFAULT_WIDTH / 2, 0, 0);
				CAMERA->SetPosition(newCamPos);

				float parallaxFactors[5] = { 0.2f, 0.4f, 0.6f, 0.8f, 1.0f };

				for (int i = 0; i < 5; i++)
				{
					if (bgLayer[i] && bgLayerDup[i])
					{
						float offsetX = newCamPos.x * parallaxFactors[i];

						// 원본 배경 위치 업데이트
						Vector3 newBgPos = bgOrigin[i] + Vector3(offsetX, 0, 0);
						bgLayer[i]->SetPosition(newBgPos);

						// 복제 배경 위치 업데이트
						Vector3 newBgDupPos = bgOriginDup[i] + Vector3(offsetX, 0, 0);
						bgLayerDup[i]->SetPosition(newBgDupPos);

						// 배경이 화면을 벗어나면 재조정하여 무한 스크롤 효과 적용
						float bgWidth = WIN_DEFAULT_WIDTH;
						if (newBgPos.x + bgWidth < newCamPos.x)
						{
							bgLayer[i]->SetPosition(bgLayerDup[i]->GetPosition() + Vector3(bgWidth, 0, 0));
							std::swap(bgLayer[i], bgLayerDup[i]);
						}
					}
				}
			}
		}

		if (testPlayer != nullptr) testPlayer->Update();
	}
}

void Scene_06::LateUpdate() {}

void Scene_06::Render()
{
	for (int i = 4; i >= 0; i--) // 배경을 뒤에서부터 순서대로 그림
	{
		if (bgLayer[i]) bgLayer[i]->Render();
		if (bgLayerDup[i]) bgLayerDup[i]->Render();
	}

	if (tmap != nullptr) tmap->Render();

	for (const auto& obj : objects) if (obj != nullptr) obj->Render();

	if (testPlayer != nullptr) testPlayer->Render();
}

void Scene_06::PostRender()
{

}

void Scene_06::ImGUI()
{

}


