#include "stdafx.h"
#include "Scene_02.h"

void Scene_02::Awake()
{
	isLoaded = false;

	objects.push_back(make_shared<ColorCircle>(CENTER, Vector3(700, 700, 0), 0.0f, 100, BLUE));

	secondHand = make_shared<ColorLine>(CENTER, 345.0f, -90.0f, RED);
	minuteHand = make_shared<ColorLine>(CENTER, 300.0f, -90.0f, GREEN);
	hourHand = make_shared<ColorLine>(CENTER, 245.0f, -90.0f, YELLOW);

	sceneNameObject = make_shared<Object>("SceneNameText", CENTER + Vector3(-540, 335, 0), Vector3(100, 25, 0), 0.0f, TransformType::Rect);
	sceneNameObject->AddComponent(make_shared<Texts>());

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
	if (sceneNameObject != nullptr) sceneNameObject.reset();
}

void Scene_02::FixedUpdate() {}

void Scene_02::Update()
{
	Clock();

	if (INPUT->KeyDown(VK_F1))
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
	sceneNameObject->GetComponent<Texts>("Texts")->RenderText(L"ClockScene", sceneNameObject->GetRect(), 25.0f);
}

void Scene_02::ImGUI() {}

void Scene_02::Clock()
{
	// 현재 로컬 시간 가져오기
	SYSTEMTIME currentTime = TIME->GetLocalPCTime();

	// 초침 각도 계산 (1초당 6도 이동)
	// 기존: float secondAngle = -90.0f + currentTime.wSecond * 6.0f;
	// 반전: (90 - 초 * 6)
	float secondAngle = 90.0f - currentTime.wSecond * 6.0f;

	// 분침 각도 계산 (1분당 6도 이동, 초당 미세 이동)
	float minuteAngle = 90.0f - (currentTime.wMinute * 6.0f) - (currentTime.wSecond * 0.1f);

	// 시침 각도 계산 (1시간당 30도 이동, 분당 미세 이동)
	float hourAngle = 90.0f - ((currentTime.wHour % 12) * 30.0f) - (currentTime.wMinute * 0.5f);

	// 각 바늘의 회전값을 절대값으로 설정
	if (secondHand != nullptr) secondHand->SetRotation(secondAngle);
	if (minuteHand != nullptr) minuteHand->SetRotation(minuteAngle);
	if (hourHand != nullptr) hourHand->SetRotation(hourAngle);
}
