#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake()
{
}

void Scene_03::Start()
{
	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(100, 100, 0), 0.0f, TexturePath + L"Tree.png"));
}

void Scene_03::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
}

void Scene_03::FixedUpdate()
{
}

void Scene_03::Update()
{
	if (KEYBOARD->Press('E'))
	{
		SCENE->LoadScene("ClockScene");
	}

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_03::LateUpdate()
{
}

void Scene_03::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_03::PostRender()
{
}

void Scene_03::ImGUI()
{
}
