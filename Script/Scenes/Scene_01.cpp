#include "stdafx.h"
#include "Scene_01.h"

void Scene_01::Awake() {}

void Scene_01::Start()
{
	objects.push_back(make_shared<ColorFilledCircle>(CENTER + Vector3(-250, 0, 0), Vector3(100, 100, 0), 0.0f, 100, YELLOW));
	objects.push_back(make_shared<ColorCircle>(CENTER + Vector3(-100, 0, 0), Vector3(100, 100, 0), 0.0f, 100, GREEN));
	objects.push_back(make_shared<ColorLine>(CENTER + Vector3(150, 0, 0), 300.0f, 0.0f, BLUE));
	objects.push_back(make_shared<ColorRect>(CENTER + Vector3(50, 0, 0), Vector3(100, 100, 0), 0.0f, RED));

	objects.back()->AddComponent(make_shared<MoveComponent>());

	CAMERA->SetTarget(objects[3].get());

	//objects.back()->AddComponent(make_shared<BoxCollider>("Rect", Vector3(0, 0, 0), Vector3(150, 150, 0), RED));
	//objects[1]->AddComponent(make_shared<BoxCollider>("Circle", Vector3(0, 0, 0), Vector3(100, 100, 0)));

	//COLLISION->AddCollider(objects[3]->GetComponent<BoxCollider>("Rect"));
	//COLLISION->AddCollider(objects[1]->GetComponent<BoxCollider>("Circle"));

	UIs.push_back(make_shared<UIElement>(0.1f, 0.2f, 0.3f, 0.1f));
}

void Scene_01::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
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

	if (KEYBOARD->Down(VK_F1))
	{
		SCENE->LoadScene("ClockScene");
	}

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_01::LateUpdate() {}

void Scene_01::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_01::PostRender() 
{
	FONT->RenderText(L"GeometryScene", UIs[0]->GetRect(gWinWidth, gWinHeight), 35.0f);
}

void Scene_01::ImGUI() {}
