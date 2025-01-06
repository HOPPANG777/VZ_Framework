#include "stdafx.h"
#include "Scene_02.h"

void Scene_02::Awake() {}

void Scene_02::Start() 
{
	objects.push_back(make_shared<ColorCircle>(CENTER, Vector3(700, 700, 0), 0.0f, 100, BLUE));
	objects.push_back(make_shared<ColorLine>(CENTER, 345.0f, -90.0f, RED));
	objects.push_back(make_shared<ColorLine>(CENTER, 300.0f, -90.0f, GREEN));
	objects.push_back(make_shared<ColorLine>(CENTER, 245.0f, -90.0f, YELLOW));
}

void Scene_02::Destroy() 
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
}

void Scene_02::FixedUpdate() {}

void Scene_02::Update() 
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	const float hourAngle = (360.0f / 12) 
		* (time.wHour) + (time.wMinute * 0.5f);

	const float minutAngle = (360.0f / 60) 
		* (time.wMinute);

	const float secondAngle = (360.0f / 60) 
		* (time.wSecond) + (360.0f / 60000) * (time.wMilliseconds);

	if (objects[1] != nullptr) objects[1]->Rotate(secondAngle);
	if (objects[2] != nullptr) objects[1]->Rotate(minutAngle);
	if (objects[3] != nullptr) objects[1]->Rotate(hourAngle);
	

	if (KEYBOARD->Press('E'))
	{
		SCENE->LoadScene("TextureScene");
	}

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_02::LateUpdate() {}

void Scene_02::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_02::PostRender() {}

void Scene_02::ImGUI() {}
