#include "stdafx.h"
#include "Scene_04.h"

void Scene_04::Awake()
{
	isLoaded = false;

	objects.push_back(make_shared<TextureRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"TitleScreen.png"));

	startButtonText = make_shared<Texts>();

	startButton = make_shared<Button>(Vector3(1135, 300, 0), Vector3(150, 50, 0), 0.0f, TexturePath + L"WhiteImage.png");

	//startButtonText->AttachToObject(startButton.get());

	isLoaded = true;
}

void Scene_04::Start()
{
	startButton->SetOnClick([this]() { SCENE->LoadScene("GeometryScene"); });
}

void Scene_04::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
	if (startButtonText != nullptr) startButtonText.reset();
	if (startButton != nullptr) startButton.reset();
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
	if (startButton != nullptr) startButton->Update();
}

void Scene_04::LateUpdate()
{

}

void Scene_04::Render()
{
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
	if (startButton != nullptr) startButton->Render();
}

void Scene_04::PostRender() 
{
	startButtonText->RenderText(L"Start", startButton->GetRect(), 37.0f, BLACK);
}

void Scene_04::ImGUI()
{

}
