#include "stdafx.h"
#include "Scene_01.h"

void Scene_01::Awake() {}

void Scene_01::Start()
{
	objects.push_back(make_shared<ColorFilledCircle>(CENTER + Vector2(-250, 0), Vector2(100, 100), 0.0f, 100, YELLOW));
	objects.push_back(make_shared<ColorCircle>(CENTER + Vector2(-100, 0), Vector2(100, 100), 0.0f, 100, GREEN));
	objects.push_back(make_shared<ColorLine>(CENTER + Vector2(150, 0), 300.0f, 0.0f, BLUE));
	objects.push_back(make_shared<ColorRect>(CENTER + Vector2(50, 0), Vector2(100, 100), 0.0f, RED));

	objects.back()->AddComponent(make_shared<MoveComponent>());
}

void Scene_01::Destroy()
{
	for (auto& obj : objects) obj.reset();
}

void Scene_01::FixedUpdate() {}

void Scene_01::Update()
{
	auto rect = dynamic_pointer_cast<ColorRect>(objects.back());

	if (KEYBOARD->Down('Q'))
	{
		if (rect->GetColor() == BLUE) rect->SetColor(RED);
		else if (rect->GetColor() == RED) rect->SetColor(BLUE);
	}

	for (const auto& obj : objects) obj->Update();
}

void Scene_01::LateUpdate() {}

void Scene_01::Render()
{
	for (const auto& obj : objects) obj->Render();
}

void Scene_01::PostRender() {}

void Scene_01::ImGUI() {}
