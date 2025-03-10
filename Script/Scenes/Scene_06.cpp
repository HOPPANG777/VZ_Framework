#include "stdafx.h"
#include "Scene_06.h"

void Scene_06::Awake()
{
	isLoaded = false;

	objects.push_back(make_shared<ColorLine>(CENTER + Vector3(-640, -100, 0), 5000, 0.0f, RED));

	isLoaded = true;
}

void Scene_06::Start()
{

}

void Scene_06::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
}

void Scene_06::FixedUpdate() {}

void Scene_06::Update()
{
	//if (tmap != nullptr) tmap->Update();

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_06::LateUpdate() {}

void Scene_06::Render()
{
	//if (tmap != nullptr) tmap->Render();

	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_06::PostRender()
{

}

void Scene_06::ImGUI()
{

}


