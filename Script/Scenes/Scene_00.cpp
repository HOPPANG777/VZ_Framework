#include "stdafx.h"
#include "Scene_00.h"

void Scene_00::Awake() {}

void Scene_00::Start()
{
	objects.push_back(make_shared<ColorRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, BLACK));
	objects.push_back(make_shared<JumpHoppang>(Vector3(WIN_DEFAULT_WIDTH * 0.97f, WIN_DEFAULT_HEIGHT * 0.09f, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"LoadingIcon.png"));

	LoadingText = make_shared<Texts>();

	UIs.push_back(make_shared<UIElement>(0.9f, 1.0f, 0.2f, 0.2f, -75, -75, 20, 20));
}

void Scene_00::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
}

void Scene_00::FixedUpdate() {}

void Scene_00::Update() 
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_00::LateUpdate() {}

void Scene_00::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_00::PostRender() 
{
	LoadingText->RenderText(L"불러오는 중...", UIs[0]->GetRect(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT), 25.0f);
}

void Scene_00::ImGUI() {}