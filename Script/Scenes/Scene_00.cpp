#include "stdafx.h"
#include "Scene_00.h"

void Scene_00::Awake() {}

void Scene_00::Start()
{
	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Fallout76.jpg"));
	objects.push_back(make_shared<TextureRect>(Vector3(WIN_DEFAULT_WIDTH * 0.9f, CENTER_Y / 5, 0), Vector3(200, 100, 0), 0.0f, TexturePath + L"SmolAmeLoading Icon.png"));
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

void Scene_00::PostRender() {}

void Scene_00::ImGUI() {}
