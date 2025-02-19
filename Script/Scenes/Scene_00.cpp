#include "stdafx.h"
#include "Scene_00.h"

void Scene_00::Awake() {}

void Scene_00::Start()
{
	isLoaded = false;

	objects.push_back(make_shared<ColorRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, BLACK));
	objects.push_back(make_shared<JumpHoppang>(Vector3(WIN_DEFAULT_WIDTH * 0.93f, WIN_DEFAULT_HEIGHT * 0.17f, 0), Vector3(200, 200, 0), 0.0f, TexturePath + L"LoadingIcon.png"));

	LoadingObject = make_shared<Object>("LoadingText", Vector3(510, 370, 0), Vector3(300, 100, 0), 0.0f, TransformType::Rect);

	LoadingText = make_shared<Texts>();

	CAMERA->Reset();

	isLoaded = true;
}

void Scene_00::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
	if (LoadingObject != nullptr) LoadingObject.reset();
	if (LoadingText != nullptr) LoadingText.reset();
}

void Scene_00::FixedUpdate() {}

void Scene_00::Update() 
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
	if (LoadingObject != nullptr) LoadingObject->Update();
}

void Scene_00::LateUpdate() {}

void Scene_00::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
	if (LoadingObject != nullptr) LoadingObject->Render();
}

void Scene_00::PostRender() 
{
	LoadingText->RenderText(L"불러오는 중", LoadingObject->GetRect(), 25.0f);
}

void Scene_00::ImGUI() {}