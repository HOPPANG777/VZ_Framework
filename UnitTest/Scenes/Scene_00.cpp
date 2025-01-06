#include "stdafx.h"
#include "Scene_00.h"

void Scene_00::Awake() {}

void Scene_00::Start()
{
	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"Loading.png"));
	objects.push_back(make_shared<TextureRect>(CENTER + Vector3(-400, -350, 0), Vector3(200, 100, 0), 0.0f, TexturePath + L"LoadingIcon.png"));
	objects.push_back(make_shared<ColorRect>(CENTER - Vector3(0, CENTER_Y_1_5, 0), Vector3(CENTER_X, 40, 0), 0.0f, WHITE));
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
