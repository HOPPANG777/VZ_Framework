#include "stdafx.h"
#include "Scenes/SceneList.h"
#include "Program.h"

#include "Systems/Window.h"

float gWinWidth = WIN_DEFAULT_WIDTH;
float gWinHeight = WIN_DEFAULT_HEIGHT;
HWND gHandle;

Program::Program() {}

Program::~Program() {}

void Program::Awake() {}

void Program::Start()
{
	//카메라 세팅
	CAMERA->Init();

	//Scene 세팅
	{
		SCENE->AddScene(make_shared<Scene_01>());

		SCENE->LoadScene(0);
	}
}

void Program::Update()
{
	CAMERA->Update();

	SCENE->Update();
}

void Program::Render()
{
	CAMERA->Render();

	SCENE->Render();
}

void Program::PostRender() 
{
	SCENE->PostRender();
}

void Program::ImGUI()
{
	SCENE->ImGUI();
}

int APIENTRY wWinMain
(
	_In_ HINSTANCE hInstance, // 윈도우 실행을 위한 고유값
	_In_opt_ HINSTANCE hPrevInstance,	
	_In_ LPWSTR    lpCmdLine, // 커맨드 전달인자
	_In_ int       nCmdShow
)
{
	WinDesc desc;
	desc.appName = L"UnitTest";	// 창 이름
	desc.instance = hInstance;	
	desc.handle = nullptr;
	desc.width = gWinWidth;
	desc.height = gWinHeight;

	auto window = make_unique<Window>(desc);
	WPARAM wParam = window->Run();

	return int(wParam);
}

