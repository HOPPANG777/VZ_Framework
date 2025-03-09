#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake() 
{
	isLoaded = false;

	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(237, 624, 0), 0.0f, TexturePath + L"Mirko1.png"));

	sceneNameObject = make_shared<Object>("SceneNameText", CENTER + Vector3(-540, 335, 0), Vector3(100, 25, 0), 0.0f, TransformType::Rect);
	sceneNameObject->AddComponent(make_shared<Texts>());

	isLoaded = true;
}

void Scene_03::Start()
{

}

void Scene_03::Destroy()
{
	if (sceneNameObject != nullptr) sceneNameObject.reset();
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
}

void Scene_03::FixedUpdate() {}

void Scene_03::Update()
{
	if (INPUT->KeyDown(VK_F1))
	{
		SCENE->LoadScene("GeometryScene");
	}

	if (sceneNameObject != nullptr) sceneNameObject->Update();
	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_03::LateUpdate() {}

void Scene_03::Render()
{
	if (sceneNameObject != nullptr) sceneNameObject->Render();
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_03::PostRender() 
{
	sceneNameObject->GetComponent<Texts>("Texts")->RenderText(L"TextureScene", sceneNameObject->GetRect(), 25.0f);
}

void Scene_03::ImGUI() {}
