#include "stdafx.h"
#include "Scene_00.h"

void Scene_00::Awake() {}

void Scene_00::Start()
{
	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"TowardsTheDemon.png"));
	objects.push_back(make_shared<JumpHoppang>(Vector3(WIN_DEFAULT_WIDTH * 0.94f, CENTER_Y / 3.5f, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"LoadingIcon.png"));

	UIs.push_back(make_shared<UIElement>(0.1f, 0.2f, 0.3f, 0.1f));
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
	FONT->RenderText(L"Hello, World!", UIs[0]->GetRect(gWinWidth, gWinHeight), 24.0f);
}

void Scene_00::ImGUI() {}