#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake() {}

void Scene_03::Start()
{
	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(352, 500, 0), 0.0f, TexturePath + L"Tree.png"));

	sceneName = make_shared<Texts>();

	sceneNameObject = make_shared<Object>("SceneNameText", Vector3(-430, -255, 0), Vector3(300, 100, 0), 0.0f, TransformType::Rect);
}

void Scene_03::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
	if (sceneName != nullptr) sceneName.reset();
	if (sceneNameObject != nullptr) sceneNameObject.reset();
}

void Scene_03::FixedUpdate() {}

void Scene_03::Update()
{
	if (KEYBOARD->Down(VK_F1))
	{
		SCENE->LoadScene("GeometryScene");
	}

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_03::LateUpdate() {}

void Scene_03::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_03::PostRender() 
{
	sceneName->RenderText(L"TextureScene", sceneNameObject->GetRect(), 25.0f);
}

void Scene_03::ImGUI() {}
