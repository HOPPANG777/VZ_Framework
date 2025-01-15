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

	if (KEYBOARD->Down('E'))
	{
		SCENE->LoadScene("ClockScene");
	}

	if (KEYBOARD->Down(VK_F4))
	{
		if (isFullScreen == false) GRAPHICS->SetFullScreen(isFullScreen = true);
		else if (isFullScreen == true) GRAPHICS->SetFullScreen(isFullScreen = false);
	}

	for (const auto& ui : sceneUIElements) RECT rect = ui.GetRect(gWinWidth, gWinHeight);

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_01::LateUpdate() {}

void Scene_01::Render()
{
	for (const auto& ui : sceneUIElements) RECT rect = ui.GetRect(gWinWidth, gWinHeight);

	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_01::PostRender() 
{
	RECT textRect;
	textRect.left = 100;
	textRect.top = 50;
	textRect.right = 400;
	textRect.bottom = 150;

	FONT->RenderText(L"GeometryScene", textRect, 35.0f);
}

void Scene_01::ImGUI() {}
