#include "stdafx.h"
#include "Program.h"
#include "Script/Editor/EditorList.h"

#include "Systems/Window.h"

// $(SolutionDir)_Build\$(Platform)\$(Configuration)\  //$(ProjectName)\ // 이걸 Client 프로젝트 설정 디버깅에 입력하시오
// call "$(SolutionDir)CopyData.bat" "$(SolutionDir)_Build\$(Platform)\$(Configuration)" $(Platform) // 이걸 Client 프로젝트 설정 빌드 후 이벤트에 입력하시오

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
	CAMERA->Init();

	//Scene 세팅
	{
		SCENE->AddScene("MapTools", make_shared<MapTools>());

		SCENE->LoadScene("MapTools");
	}
}

void Program::Start()
{
	SCENE->Start();
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
	desc.appName = L"Tools";	// 창 이름
	desc.instance = hInstance;	
	desc.handle = nullptr;
	desc.width = gWinWidth;
	desc.height = gWinHeight;
	desc.icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_Tools));

	auto window = make_unique<Window>(desc);
	WPARAM wParam = window->Run();

	return int(wParam);
}

