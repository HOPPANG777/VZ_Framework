#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake() {}

void Scene_03::Start()
{
	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(352, 500, 0), 0.0f, TexturePath + L"Tree.png"));

	sceneName = make_shared<Texts>();

	UIs.push_back(make_shared<UIElement>(0.01f, 0.01f, 0.5f, 0.5f));
}

void Scene_03::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
	if (sceneName != nullptr) sceneName.reset();
}

void Scene_03::FixedUpdate() {}

void Scene_03::Update()
{
	if (KEYBOARD->Down(VK_F1))
	{
		SCENE->LoadScene("GeometryScene");
	}

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_03::LateUpdate() {}

void Scene_03::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_03::PostRender() 
{
	sceneName->RenderText(L"TextureScene", UIs[0]->GetRect(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT), 25.0f);
}

void Scene_03::ImGUI() {}
