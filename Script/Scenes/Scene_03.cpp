#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake() 
{
	isLoaded = false;

	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(237, 624, 0), 0.0f, TexturePath + L"Mirko1.png"));

	sceneNameObject = make_shared<Object>("SceneNameText", Vector3(100, 695, 0), Vector3(100, 25, 0), 0.0f, TransformType::Rect);
	
	sceneName = make_shared<Texts>();
	sceneNameObject->AddComponent(sceneName);

	isLoaded = true;
}

void Scene_03::Start()
{

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
	if (sceneNameObject != nullptr) sceneNameObject->Update();
}

void Scene_03::LateUpdate() {}

void Scene_03::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
	if (sceneNameObject != nullptr) sceneNameObject->Render();
}

void Scene_03::PostRender() 
{
	sceneName->RenderText(L"TextureScene", sceneNameObject->GetRect(), 25.0f);
}

void Scene_03::ImGUI() {}
