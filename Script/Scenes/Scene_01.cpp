#include "stdafx.h"
#include "Scene_01.h"

void Scene_01::Awake()
{
	isLoaded = false;

	{
		objects.push_back(std::make_shared<TextureRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"UI/NordicLumberJacTitleBackground.png"));
		objects.push_back(std::make_shared<TextureRect>(CENTER + Vector3(-260, 210, 0), Vector3(700, 700, 0), 0.0f, TexturePath + L"UI/NordicLumberJackTitle.png"));
		objects.push_back(std::make_shared<TextureRect>(CENTER + Vector3(495, -120, 0), Vector3(150, 100, 0), 0.0f, TexturePath + L"UI/Button02.png", TransformType::Rect));
		objects.push_back(std::make_shared<TextureRect>(CENTER + Vector3(495, -180, 0), Vector3(150, 100, 0), 0.0f, TexturePath + L"UI/Button02.png", TransformType::Rect));
		objects.push_back(std::make_shared<TextureRect>(CENTER + Vector3(495, -240, 0), Vector3(150, 100, 0), 0.0f, TexturePath + L"UI/Button02.png", TransformType::Rect));
		objects[2]->AddComponent(std::make_shared<Texts>());
		objects[3]->AddComponent(std::make_shared<Texts>());
		objects[4]->AddComponent(std::make_shared<Texts>());
	}

	{
		startButton = std::make_shared<TextureRect>(CENTER + Vector3(495, -60, 0), Vector3(150, 100, 0), 0.0f, TexturePath + L"UI/Button02.png", TransformType::Rect);
		startButton->AddComponent(std::make_shared<Texts>());
	}
	
	isLoaded = true;
}

void Scene_01::Start()
{
	const auto& startButtonComp = std::make_shared<Button>();
	startButtonComp->SetOnClick([this]() { SceneManager::Get()->LoadScene("StartStage"); });
	startButton->AddComponent(startButtonComp);
	const auto& exitButtonComp = std::make_shared<Button>();
	exitButtonComp->SetOnClick([]() { PostQuitMessage(0); });
	objects[4]->AddComponent(exitButtonComp);
}

void Scene_01::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
	if (startButton != nullptr) startButton.reset();
}

void Scene_01::FixedUpdate()
{

}

void Scene_01::Update()
{
	if (Input::Get()->KeyDown(VK_F1))
	{
		SceneManager::Get()->LoadScene("GeometryScene");
	}

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
	if (startButton != nullptr) startButton->Update();
}

void Scene_01::LateUpdate()
{

}

void Scene_01::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
	if (startButton != nullptr) startButton->Render();
}

void Scene_01::PostRender() 
{
	startButton->GetComponent<Texts>("Texts")->RenderText(L"Start", startButton->GetRect(), 37.0f, WHITE);
	objects[2]->GetComponent<Texts>("Texts")->RenderText(L"Setting", objects[2]->GetRect(), 37.0f, WHITE);
	objects[3]->GetComponent<Texts>("Texts")->RenderText(L"Credit", objects[3]->GetRect(), 37.0f, WHITE);
	objects[4]->GetComponent<Texts>("Texts")->RenderText(L"Quit", objects[4]->GetRect(), 37.0f, WHITE);
}

void Scene_01::ImGUI()
{

}
