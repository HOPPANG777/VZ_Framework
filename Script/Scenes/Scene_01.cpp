#include "stdafx.h"
#include "Scene_01.h"

void Scene_01::Awake()
{
	isLoaded = false;

	{
		objects.push_back(make_shared<TextureRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"UI/TitleScreen2.png"));
		objects.push_back(make_shared<TextureRect>(CENTER + Vector3(-260, 210, 0), Vector3(700, 700, 0), 0.0f, TexturePath + L"UI/NordicLumberJackTitle.png"));
	}

	{
		startButton = make_shared<TextureRect>(CENTER + Vector3(495, -60, 0), Vector3(150, 100, 0), 0.0f, TexturePath + L"UI/Button01.png", TransformType::Rect);
		startButton->AddComponent(make_shared<Texts>());
	}
	
	isLoaded = true;
}

void Scene_01::Start()
{
	std::shared_ptr<Button> startButtonComp = make_shared<Button>();
	startButtonComp->SetOnClick([this]() { SCENE->LoadScene("StartStage"); });
	startButton->AddComponent(startButtonComp);
}

void Scene_01::Destroy()
{
	for (std::shared_ptr<Object>& obj : objects) if (obj != nullptr) obj.reset();
	if (startButton != nullptr) startButton.reset();
}

void Scene_01::FixedUpdate()
{

}

void Scene_01::Update()
{
	if (INPUT->KeyDown(VK_F1))
	{
		SCENE->LoadScene("GeometryScene");
	}

	for (const std::shared_ptr<Object>& obj : objects) if (obj != nullptr) obj->Update();
	if (startButton != nullptr) startButton->Update();
}

void Scene_01::LateUpdate()
{

}

void Scene_01::Render()
{
	for (const std::shared_ptr<Object>& obj : objects) if (obj != nullptr) obj->Render();
	if (startButton != nullptr) startButton->Render();
}

void Scene_01::PostRender() 
{
	startButton->GetComponent<Texts>("Texts")->RenderText(L"Start", startButton->GetRect(), 37.0f, WHITE);
}

void Scene_01::ImGUI()
{

}
