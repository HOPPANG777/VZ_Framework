#include "stdafx.h"
#include "Scene_04.h"

void Scene_04::Awake()
{
}

void Scene_04::Start()
{
	isLoaded = false;

	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"TitleScreen.png"));

	isLoaded = true;
}

void Scene_04::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
}

void Scene_04::FixedUpdate()
{

}

void Scene_04::Update()
{
	if (KEYBOARD->Down(VK_F1))
	{
		SCENE->LoadScene("GeometryScene");
	}

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_04::LateUpdate()
{

}

void Scene_04::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_04::PostRender() 
{

}

void Scene_04::ImGUI()
{

}
