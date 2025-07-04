#include "stdafx.h"
#include "Scene_00.h"

void Scene_00::Awake() {}

void Scene_00::Start()
{
	isLoaded = false;

	{
		objects.push_back(std::make_shared<ColorRect>(Values::GetCenter(), Vector3(Values::GetWinWidth(), Values::GetWinHeight(), 0), 0.0f, BLACK));
		objects.push_back(std::make_shared<JumpHoppang>(Values::GetCenter() + Vector3(Values::GetWinWidth() * 0.93f, Values::GetWinHeight() * 0.17f, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"UI/LoadingIcon.png"));
	}
	
	{
		LoadingTextObject = std::make_shared<GameObject>("LoadingText", Values::GetCenter() + Vector3(380, -360, 0), Vector3(100, 25, 0), 0.0f, TransformType::Rect);
		LoadingTextObject->AddComponent(std::make_shared<Texts>());
	}
	
	Camera::Get()->Reset();

	isLoaded = true;
}

void Scene_00::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
	if (LoadingTextObject != nullptr) LoadingTextObject.reset();
}

void Scene_00::FixedUpdate() {}

void Scene_00::Update() 
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
	if (LoadingTextObject != nullptr) LoadingTextObject->Update();
}

void Scene_00::LateUpdate() {}

void Scene_00::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
	if (LoadingTextObject != nullptr) LoadingTextObject->Render();
}

void Scene_00::PostRender() 
{
	LoadingTextObject->GetComponent<Texts>("Texts")->RenderText(L"불러오는 중", LoadingTextObject->GetRect(), 25.0f);
}

void Scene_00::ImGUI() {}