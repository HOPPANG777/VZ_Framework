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
    // 현재 로컬 시간 가져오기
    SYSTEMTIME currentTime = TIME->GetLocalPCTime();

    // 초침 각도 계산 (1초당 6도 이동)
    float secondAngle = -90.0f + currentTime.wSecond * 6.0f;

    // 분침 각도 계산 (1분당 6도 이동, 초당 미세 이동)
    float minuteAngle = -90.0f + (currentTime.wMinute * 6.0f) + (currentTime.wSecond * 0.1f);

    // 시침 각도 계산 (1시간당 30도 이동, 분당 미세 이동)
    float hourAngle = -90.0f + (currentTime.wHour % 12) * 30.0f + (currentTime.wMinute * 0.5f);

    // 시계 바늘 회전 업데이트
    if (objects[1] != nullptr) objects[1]->Rotate(secondAngle - objects[1]->GetRotation());  // 초침
    if (objects[2] != nullptr) objects[2]->Rotate(minuteAngle - objects[2]->GetRotation());  // 분침
    if (objects[3] != nullptr) objects[3]->Rotate(hourAngle - objects[3]->GetRotation());  // 시침

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