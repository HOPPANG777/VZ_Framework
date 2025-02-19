#include "stdafx.h"
#include "Scene_01.h"

void Scene_01::Awake() 
{
	isLoaded = false;

	objects.push_back(make_shared<ColorFilledCircle>(CENTER + Vector3(-250, 0, 0), Vector3(100, 100, 0), 0.0f, 100, YELLOW));
	objects.push_back(make_shared<ColorCircle>(CENTER + Vector3(-100, 0, 0), Vector3(100, 100, 0), 0.0f, 100, GREEN));
	objects.push_back(make_shared<ColorLine>(CENTER + Vector3(150, 0, 0), 300.0f, 0.0f, BLUE));
	red = make_shared<ColorRect>(CENTER + Vector3(50, 0, 0), Vector3(100, 100, 0), 0.0f, RED);

	red->AddComponent(make_shared<MoveComponent>());

	sceneName = make_shared<Texts>();
	testTyping = make_shared<TypingText>();
	testShake = make_shared<ShakingText>();

	sceneNameObject = make_shared<Object>("SceneNameText", Vector3(-430, -255, 0), Vector3(300, 100, 0), 0.0f, TransformType::Rect);
	testTypingObject = make_shared<Object>("TestTypingText", Vector3(0, -100, 0), Vector3(500, 100, 0), 0.0f, TransformType::Rect);
	testShakeObject = make_shared<Object>("TestShakeText", Vector3(0, 200, 0), Vector3(300, 100, 0), 0.0f, TransformType::Rect);

	isLoaded = true;
}

void Scene_01::Start()
{
	SOUNDS->AddSound("BGM", SoundPath + L"bgm.wav", true);

	SOUNDS->Play("BGM", 0.2f);

	CAMERA->SetTarget(red.get());
}

void Scene_01::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
	//for (auto& uis : UIs) if (uis != nullptr) uis.reset();
	if (red != nullptr) red.reset();
	if (sceneName != nullptr) sceneName.reset();
	if (testTyping != nullptr) testTyping.reset();
	if (testShake != nullptr) testShake.reset();
	if (sceneNameObject != nullptr) sceneNameObject.reset();
	if (testTypingObject != nullptr) testTypingObject.reset();
	if (testShakeObject != nullptr) testShakeObject.reset();
	SOUNDS->RemoveSound("BGM");
	
}

void Scene_01::FixedUpdate() {}

void Scene_01::Update()
{
	auto rect = dynamic_pointer_cast<ColorRect>(red);

	if (KEYBOARD->Down('Q'))
	{
		if (rect->GetColor() == BLUE) rect->SetColor(RED);
		else if (rect->GetColor() == RED) rect->SetColor(BLUE);
	}

	if (KEYBOARD->Down(VK_F1))
	{
		SCENE->LoadScene("ClockScene");
	}

	if (testTyping != nullptr) testTyping->Update();

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
	if (red != nullptr) red->Update();
}

void Scene_01::LateUpdate() {}

void Scene_01::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
	if (red != nullptr) red->Render();
}

void Scene_01::PostRender() 
{
	sceneName->RenderText(L"GeometryScene", sceneNameObject->GetRect(), 25.0f);
	testTyping->RenderText(L"안녕하세요 조승영입니다 이건 테스트입니다.", testTypingObject->GetRect(), 25.0f);
	testShake->RenderText(L"그라라라라라라라라.", testShakeObject->GetRect(), 25.0f);
}

void Scene_01::ImGUI() {}

//objects.back()->AddComponent(make_shared<BoxCollider>("Rect", Vector3(0, 0, 0), Vector3(150, 150, 0), RED));
//objects[1]->AddComponent(make_shared<BoxCollider>("Circle", Vector3(0, 0, 0), Vector3(100, 100, 0)));

//COLLISION->AddCollider(objects[3]->GetComponent<BoxCollider>("Rect"));
//COLLISION->AddCollider(objects[1]->GetComponent<BoxCollider>("Circle"));