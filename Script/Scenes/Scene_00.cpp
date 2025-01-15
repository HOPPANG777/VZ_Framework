#include "stdafx.h"
#include "Scene_00.h"

void Scene_00::Awake() {}

void Scene_00::Start()
{
	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"TowardsTheDemon.png"));
	objects.push_back(make_shared<JumpHoppang>(Vector3(WIN_DEFAULT_WIDTH * 0.94f, CENTER_Y / 3.5f, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"LoadingIcon.png"));
}

void Scene_00::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
}

void Scene_00::FixedUpdate() {}

void Scene_00::Update() 
{
	// 해상도에 따라 UI 업데이트 (예시)
	for (const auto& ui : sceneUIElements) RECT rect = ui.GetRect(gWinWidth, gWinHeight); // rect를 사용하여 렌더링

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_00::LateUpdate() {}

void Scene_00::Render()
{
    // rect를 기반으로 Direct2D/Direct3D로 렌더링
    for (const auto& ui : sceneUIElements) RECT rect = ui.GetRect(gWinWidth, gWinHeight);

	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_00::PostRender() 
{
	RECT textRect;
	textRect.left = 100;
	textRect.top = 50;
	textRect.right = 400;
	textRect.bottom = 150;

	FONT->RenderText(L"Hello, World!", textRect, 24.0f);
}

void Scene_00::ImGUI() {}