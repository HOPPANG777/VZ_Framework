#include "stdafx.h"
#include "Scene_02.h"

void Scene_02::Awake() {}

void Scene_02::Start() 
{
	objects.push_back(make_shared<ColorCircle>(CENTER, Vector2(700, 700), 0.0f, 100, BLUE));
	objects.push_back(make_shared<ColorLine>(CENTER, 345.0f, -90.0f, RED));
	objects.push_back(make_shared<ColorLine>(CENTER, 300.0f, -90.0f, GREEN));
	objects.push_back(make_shared<ColorLine>(CENTER, 245.0f, -90.0f, YELLOW));
}

void Scene_02::Destroy() 
{
	for (auto& obj : objects) obj.reset();
}

void Scene_02::FixedUpdate() {}

void Scene_02::Update() 
{
	for (const auto& obj : objects) obj->Update();
}

void Scene_02::LateUpdate() {}

void Scene_02::Render()
{
	for (const auto& obj : objects) obj->Render();
}

void Scene_02::PostRender() {}

void Scene_02::ImGUI() {}
