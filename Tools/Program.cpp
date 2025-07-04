#include "stdafx.h"
#include "Program.h"
#include "Editor/EditorList.h"

#include "Systems/Window.h"
#include "Managers/RendererManager.h"
#include "Enums/RenderAPI.h"

// 실행 시 윈도우 크기 설정
float gWinWidth = WIN_DEFAULT_WIDTH;
float gWinHeight = WIN_DEFAULT_HEIGHT;
HWND gHandle;

Program::Program() {}
Program::~Program() {}

void Program::Awake()
{
    RenderStates::Init();
    Camera::Get()->Init();

    // 씬 설정
    {
        // 통합 에디터 씬을 등록하고 바로 로드합니다.
        SceneManager::Get()->AddScene("IntegratedEditor", std::make_shared<IntegratedEditor>());
        SceneManager::Get()->LoadScene("IntegratedEditor");
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

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    WinDesc desc;
    desc.appName = L"Tools";
    desc.instance = hInstance;
    desc.handle = nullptr;
    desc.width = gWinWidth;
    desc.height = gWinHeight;
    desc.icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_Tools));

    auto window = std::make_unique<Window>(desc);

    // 렌더러 생성 (DirectX11)
    RendererManager::Create(RenderAPI::DirectX11);
    WPARAM wParam = window->Run();

    return static_cast<int>(wParam);
}
