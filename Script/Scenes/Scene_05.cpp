#include "stdafx.h"
#include "Scene_05.h"

void Scene_05::Awake()
{
	isLoaded = false;
	
	// Background
	{
		std::shared_ptr<ParallaxBackground> parallax = make_shared<ParallaxBackground>("Parallax", 6);

		// 배경 레이어 2개씩 생성
		for (int i = 0; i < 5; i++)
		{
			bgLayer[i] = make_shared<TextureRect>(CENTER + Vector3(0, 0, 0), Vector3(WIN_DEFAULT_WIDTH * 1.2f, WIN_DEFAULT_HEIGHT * 1.2f, 0), 0.0f, TexturePath + L"Background/StartScene/" + to_wstring(i + 1) + L".png");
			bgLayerDup[i] = make_shared<TextureRect>(CENTER + Vector3(WIN_DEFAULT_WIDTH * 1.2f, 0, 0), Vector3(WIN_DEFAULT_WIDTH * 1.2f, WIN_DEFAULT_HEIGHT * 1.2f, 0), 0.0f, TexturePath + L"Background/StartScene/" + to_wstring(i + 1) + L".png");

			bgOrigin[i] = bgLayer[i]->GetPosition();
			bgOriginDup[i] = bgLayerDup[i]->GetPosition();

			parallax->SetLayer(i, bgLayer[i], bgLayerDup[i]);
		}

		backgroundObject = make_shared<Object>("Background");
		backgroundObject->AddComponent(parallax);
	}

	isLoaded = true;
}

void Scene_05::Start()
{

}

void Scene_05::Destroy()
{
	for (auto& bg : bgLayer) if (bg != nullptr) bg.reset();
	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup.reset();
	if (backgroundObject != nullptr) backgroundObject.reset();
}

void Scene_05::FixedUpdate()
{
}

void Scene_05::Update()
{
	for (auto& bg : bgLayer) if (bg != nullptr) bg->Update();
	for (auto& bgdup : bgLayerDup) if (bgdup != nullptr) bgdup->Update();
	if (backgroundObject != nullptr) backgroundObject->Update();
}

void Scene_05::LateUpdate()
{
}

void Scene_05::Render()
{
	if (backgroundObject != nullptr) backgroundObject->Render();
}

void Scene_05::PostRender()
{
}

void Scene_05::ImGUI()
{
}
