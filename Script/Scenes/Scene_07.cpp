#include "stdafx.h"
#include "Scene_07.h"

void Scene_07::Awake()
{
	isLoaded = false;

	objects.push_back(std::make_shared<TextureRect>(Values::GetCenter(), Vector3(Values::GetWinWidth(), Values::GetWinHeight(), 0), 0.0f, TexturePath + L"UI/¸·Çû´Ù2.png"));

	isLoaded = true;
}

void Scene_07::Start()
{
}

void Scene_07::Destroy()
{
	for (auto& obj : objects) if (obj) obj.reset();
}

void Scene_07::FixedUpdate()
{
}

void Scene_07::Update()
{
	for (const auto& obj : objects) if (obj) obj->Update();
}

void Scene_07::LateUpdate()
{
}

void Scene_07::Render()
{
	for (const auto& obj : objects) if (obj) obj->Render();
}

void Scene_07::PostRender()
{
}

void Scene_07::ImGUI()
{
}
