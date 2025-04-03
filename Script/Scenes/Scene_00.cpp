#include "stdafx.h"
#include "Scene_00.h"

void Scene_00::Awake() {}

void Scene_00::Start()
{
	isLoaded = false;

	{
		objects.push_back(make_shared<ColorRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, BLACK));
		objects.push_back(make_shared<JumpHoppang>(CENTER + Vector3(WIN_DEFAULT_WIDTH * 0.93f, WIN_DEFAULT_HEIGHT * 0.17f, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"UI/LoadingIcon.png"));
	}
	
	{
		LoadingTextObject = make_shared<Object>("LoadingText", CENTER + Vector3(380, -360, 0), Vector3(100, 25, 0), 0.0f, TransformType::Rect);
		LoadingTextObject->AddComponent(make_shared<Texts>());
	}
	
	CAMERA->Reset();

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