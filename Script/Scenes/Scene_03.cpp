#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake()
{
	isLoaded = false;

	floor.push_back(make_shared<ColorLine>(CENTER + Vector3(-640, -330, 0), 10000, 0.0f, CLEAR));

	testPlayer = make_shared <PlayerUnit> (CENTER + Vector3(-550, -300, 0));



	// 배경 레이어 2개씩 생성
	for (int i = 0; i < 5; i++)
	{
		bgLayer[i] = make_shared<TextureRect>(CENTER + Vector3(0, 50, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/StartScene/" + to_wstring(i + 1) + L".png");
		bgLayerDup[i] = make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH, 50, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/StartScene/" + to_wstring(i + 1) + L".png");

		bgOrigin[i] = bgLayer[i]->GetPosition();
		bgOriginDup[i] = bgLayerDup[i]->GetPosition();
	}

	floor[0]->AddComponent(make_shared<BoxCollider>(Vector3(0, 0, 0), Vector3(10000, 1, 0), GREEN));

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

	isLoaded = true;
}

void Scene_03::Start()
{
	//CAMERA->SetTarget(testPlayer.get());
}

void Scene_03::Destroy()
{
	for (auto& floors : floor) if (floors != nullptr) floors.reset();
	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();
	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup.reset();
	if (testPlayer != nullptr) testPlayer.reset();
}

void Scene_03::FixedUpdate() {}

void Scene_03::Update()
{
	if (tmap != nullptr) tmap->Update();

	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Update();
	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();

	for (const auto& floors : floor) if (floors != nullptr) floors->Update();

	if (testPlayer != nullptr) testPlayer->Update();

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

	// 플레이어와 각 floor의 충돌 검사
	// 플레이어와 floor에 부착된 BoxCollider는 "BoxCollider"라는 이름으로 등록되었다고 가정합니다.
	auto playerCollider = testPlayer->GetComponent<BoxCollider>("BoxCollider");
	if (playerCollider)
	{
		for (const auto& f : floor)
		{
			auto floorCollider = f->GetComponent<BoxCollider>("BoxCollider");
			if (!floorCollider) continue;

			// 충돌이 발생하면 (floorCollider의 IsColliding() 사용)
			if (floorCollider->IsColliding(playerCollider.get()))
			{
				// 발판의 상단 = floor의 center.y + (floor의 size.y * 0.5)
				float floorTop = floorCollider->GetCenter().y + (floorCollider->GetSize().y * 0.5f);
				// 플레이어의 하단 = player의 center.y - (player의 size.y * 0.5)
				float playerBottom = playerCollider->GetCenter().y - (playerCollider->GetSize().y * 0.5f);
				// 만약 플레이어가 발판에 침투했다면 보정
				if (playerBottom < floorTop)
				{
					testPlayer->GetComponent<PlayerMove>("PlayerMove")->Land();
					float correction = floorTop - playerBottom;
					// 플레이어의 현재 위치를 가져와 y 값을 보정한 후 적용
					Vector3 curPos = testPlayer->GetPosition();
					testPlayer->SetPosition({ curPos.x, curPos.y + correction, curPos.z });
					// 발판과 충돌했으므로 추가적인 낙하 처리는 여기서 중단
					// (예: 점프 상태를 false로 전환하는 등의 처리는 필요에 따라 추가)
					break;
				}
			}
		}
	}

}

void Scene_03::LateUpdate() {}

void Scene_03::Render()
{
	for (int i = 4; i >= 0; i--) // 배경을 뒤에서부터 순서대로 그림
	{
		if (bgLayer[i]) bgLayer[i]->Render();
		if (bgLayerDup[i]) bgLayerDup[i]->Render();
	}

	if (tmap != nullptr) tmap->Render();

	for (const auto& floors : floor) if (floors != nullptr) floors->Render();

	if (testPlayer != nullptr) testPlayer->Render();
}

void Scene_03::PostRender()
{

}

void Scene_03::ImGUI()
{

}


