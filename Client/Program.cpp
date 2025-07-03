#include "stdafx.h"
#include "Program.h"
#include "Script/Scenes/SceneList.h"

#include "Systems/Window.h"

// $(SolutionDir)_Build\$(PlatformTarget)\$(Configuration)\  //$(ProjectName)\ // 이걸 Client 프로젝트 설정 디버깅에 입력하시오
// call "$(SolutionDir)CopyData.bat" "$(SolutionDir)_Build\$(PlatformTarget)\$(Configuration)" $(PlatformTarget) // 이걸 Client 프로젝트 설정 빌드 후 이벤트에 입력하시오

float gWinWidth = WIN_DEFAULT_WIDTH;
float gWinHeight = WIN_DEFAULT_HEIGHT;
HWND gHandle;

Program::Program() {}

Program::~Program() {}

void Program::Awake() 
{
	//다이렉트 싱글톤 세팅
	RenderStates::Init();

	//카메라 세팅
	Camera::Get()->Init();

	//Scene 세팅
	{
		SceneManager::Get()->AddScene("LoadingScene", std::make_shared<Scene_00>());
		SceneManager::Get()->AddScene("TitleScene", std::make_shared<Scene_01>());
		SceneManager::Get()->AddScene("SplashScene", std::make_shared<Scene_02>());
		SceneManager::Get()->AddScene("StartStage", std::make_shared<Scene_03>());
		SceneManager::Get()->AddScene("Stage01", std::make_shared<Scene_04>());
		SceneManager::Get()->AddScene("Stage02", std::make_shared<Scene_05>());
		SceneManager::Get()->AddScene("Stage03", std::make_shared<Scene_06>());
		SceneManager::Get()->AddScene("PuzzleStage", std::make_shared<Scene_07>());

		SceneManager::Get()->SetSplashScene("SplashScene");

		SceneManager::Get()->SetLoadingScene("LoadingScene");

		SceneManager::Get()->StartGame("StartStage");
		//SCENE->StartGame("Stage02");
	}
}

void Program::Start()
{
	SceneManager::Get()->Start();
}

void Program::Update()
{
	Camera::Get()->Update();

	SceneManager::Get()->Update();

	if (Input::Get()->KeyDown(VK_F2))
	{
		CollisionManager::Get()->RenderDebug();
	}

	if (Input::Get()->KeyDown(VK_F3))
	{
		if (isFullScreen == false) Graphics::Get()->SetFullScreen(isFullScreen = true);
		else if (isFullScreen == true) Graphics::Get()->SetFullScreen(isFullScreen = false);
	}

	if (Input::Get()->KeyDown(VK_F5))
	{
		if (Time::Get()->GetTimeScale() != 0)
		{
			Time::Get()->SetTimeScale(0);
		}
		else
		{
			Time::Get()->SetTimeScale(1);
		}
	}
}

void Program::Render()
{
	Camera::Get()->Render();

	SceneManager::Get()->Render();
}

void Program::PostRender() 
{
	SceneManager::Get()->PostRender();
}

void Program::ImGUI()
{
	SceneManager::Get()->ImGUI();
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
	desc.appName = L"NordicLumberjack";	// 창 이름
	desc.instance = hInstance;	
	desc.handle = nullptr;
	desc.width = gWinWidth;
	desc.height = gWinHeight;
	desc.icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOPPANG));

	auto window = std::make_unique<Window>(desc);
	WPARAM wParam = window->Run();

	return int(wParam);
}

