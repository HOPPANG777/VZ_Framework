#include "stdafx.h"
#include "Scene_06.h"

void Scene_06::Awake()
{
	isLoaded = false;

	// 배경 레이어 2개씩 생성
	for (int i = 0; i < 5; i++)
	{
		bgLayer[i] = make_shared<TextureRect>(CENTER + Vector3(0, 50, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/BossScene/" + to_wstring(i + 1) + L".png");
		bgLayerDup[i] = make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH, 50, 0), Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Background/BossScene/" + to_wstring(i + 1) + L".png");

		bgOrigin[i] = bgLayer[i]->GetPosition();
		bgOriginDup[i] = bgLayerDup[i]->GetPosition();
	}


	isLoaded = true;
}

void Scene_06::Start()
{
}

void Scene_06::Destroy()
{
	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();
	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup.reset();
}

void Scene_06::FixedUpdate()
{
}

void Scene_06::Update()
{
	for (const auto& bg : bgLayer) if (bg != nullptr) bg->Update();
	for (const auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();

	/*
	if (player->GetPosition().x > WIN_DEFAULT_WIDTH * 0.5 && player->GetPosition().x < WIN_DEFAULT_WIDTH * 1.5)
	{
		Vector3 newCamPos = Vector3(player->GetPosition().x - WIN_DEFAULT_WIDTH / 2, 0, 0);
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
	*/
}

void Scene_06::LateUpdate()
{
}

void Scene_06::Render()
{
	for (int i = 4; i >= 0; i--) // 배경을 뒤에서부터 순서대로 그림
	{
		if (bgLayer[i]) bgLayer[i]->Render();
		if (bgLayerDup[i]) bgLayerDup[i]->Render();
	}


}

void Scene_06::PostRender()
{
}

void Scene_06::ImGUI()
{
}
