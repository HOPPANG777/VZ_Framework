#include "stdafx.h"
#include "Scene_02.h"

void Scene_02::Awake() 
{
    isLoaded = false;

    objects.push_back(make_shared<ColorCircle>(CENTER, Vector3(700, 700, 0), 0.0f, 100, BLUE));

    secondHand = make_shared<ColorLine>(CENTER, 345.0f, -90.0f, RED);
    minuteHand = make_shared<ColorLine>(CENTER, 300.0f, -90.0f, GREEN);
    hourHand = make_shared<ColorLine>(CENTER, 245.0f, -90.0f, YELLOW);

    sceneNameObject = make_shared<Object>("SceneNameText", Vector3(100, 695, 0), Vector3(100, 25, 0), 0.0f, TransformType::Rect);
    sceneName = make_shared<Texts>();
    sceneNameObject->AddComponent(sceneName);

    isLoaded = true;
}

void Scene_02::Start() 
{

}

void Scene_02::Destroy() 
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
    if (secondHand != nullptr) secondHand.reset();
    if (minuteHand != nullptr) minuteHand.reset();
    if (hourHand != nullptr) hourHand.reset();
    if (sceneName != nullptr) sceneName.reset();
    if (sceneNameObject != nullptr) sceneNameObject.reset();
}

void Scene_02::FixedUpdate() {}

void Scene_02::Update() 
{
    Clock();

	if (KEYBOARD->Down(VK_F1))
	{
		SCENE->LoadScene("TextureScene");
	}

    for (const auto& obj : objects) if (obj != nullptr) obj->Update();

    if (secondHand != nullptr) secondHand->Update();
    if (minuteHand != nullptr) minuteHand->Update();
    if (hourHand != nullptr) hourHand->Update();
}

void Scene_02::LateUpdate() {}

void Scene_02::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
    
    if (secondHand != nullptr) secondHand->Render();
    if (minuteHand != nullptr) minuteHand->Render();
    if (hourHand != nullptr) hourHand->Render();
}

void Scene_02::PostRender() 
{
    sceneName->RenderText(L"ClockScene", sceneNameObject->GetRect(), 25.0f);
}

void Scene_02::ImGUI() {}

void Scene_02::Clock()
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
    if (secondHand != nullptr) secondHand->Rotate(secondAngle - secondHand->GetRotation());  // 초침
    if (minuteHand != nullptr) minuteHand->Rotate(minuteAngle - minuteHand->GetRotation());  // 분침
    if (hourHand != nullptr) hourHand->Rotate(hourAngle - hourHand->GetRotation());  // 시침
}
