#include "stdafx.h"
#include "Scene_01.h"

void Scene_01::Awake() {}

void Scene_01::Start()
{
	objects.push_back(make_shared<ColorFilledCircle>(CENTER + Vector3(-250, 0, 0), Vector3(100, 100, 0), 0.0f, 100, YELLOW));
	objects.push_back(make_shared<ColorCircle>(CENTER + Vector3(-100, 0, 0), Vector3(100, 100, 0), 0.0f, 100, GREEN));
	objects.push_back(make_shared<ColorLine>(CENTER + Vector3(150, 0, 0), 300.0f, 0.0f, BLUE));
	objects.push_back(make_shared<ColorRect>(CENTER + Vector3(50, 0, 0), Vector3(100, 100, 0), 0.0f, RED));

	objects[3]->AddComponent(make_shared<MoveComponent>());

	CAMERA->SetTarget(objects[3].get());

	//objects.back()->AddComponent(make_shared<BoxCollider>("Rect", Vector3(0, 0, 0), Vector3(150, 150, 0), RED));
	//objects[1]->AddComponent(make_shared<BoxCollider>("Circle", Vector3(0, 0, 0), Vector3(100, 100, 0)));

	//COLLISION->AddCollider(objects[3]->GetComponent<BoxCollider>("Rect"));
	//COLLISION->AddCollider(objects[1]->GetComponent<BoxCollider>("Circle"));

	SceneName = make_shared<Texts>();
	TestTyping = make_shared<TypingText>();
	
	UIs.push_back(make_shared<UIElement>(0.01f, 0.01f, 0.5f, 0.5f));
	UIs.push_back(make_shared<UIElement>(0.31f, 0.7f, 0.5f, 0.5f));
}

void Scene_01::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
	for (auto& uis : UIs) if (uis != nullptr) uis.reset();
	if (SceneName != nullptr) SceneName.reset();
	if (TestTyping != nullptr) TestTyping.reset();
}

void Scene_01::FixedUpdate() {}

void Scene_01::Update()
{
	auto rect = dynamic_pointer_cast<ColorRect>(objects.back());

	if (KEYBOARD->Down('Q'))
	{
		if (rect->GetColor() == BLUE) rect->SetColor(RED);
		else if (rect->GetColor() == RED) rect->SetColor(BLUE);
	}

	if (KEYBOARD->Down(VK_F1))
	{
		SCENE->LoadScene("ClockScene");
	}

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();

	TestTyping->Update();
}

void Scene_01::LateUpdate() {}

void Scene_01::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_01::PostRender() 
{
	SceneName->RenderText(L"GeometryScene", UIs[0]->GetRect(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT), 25.0f);
	TestTyping->RenderText(L"안녕하세요 조승영입니다 이건 테스트입니다.", UIs[1]->GetRect(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT), 25.0f);
}

void Scene_01::ImGUI() {}
