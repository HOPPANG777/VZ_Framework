#include "stdafx.h"
#include "Program.h"
#include "Script/Scenes/SceneList.h"

#include "Systems/Window.h"
#include "Managers/RendererManager.h"
#include "Enums/RenderAPI.h"

float gWinWidth = WIN_DEFAULT_WIDTH;
float gWinHeight = WIN_DEFAULT_HEIGHT;
HWND gHandle;

Program::Program() {}
Program::~Program() {}

void Program::Awake()
{
    RenderStates::Init();
    Camera::Get()->Init();

    // 게임 씬 설정
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
        if (!isFullScreen)
            Graphics::Get()->SetFullScreen(isFullScreen = true);
        else
            Graphics::Get()->SetFullScreen(isFullScreen = false);
    }

    if (Input::Get()->KeyDown(VK_F5))
    {
        if (Time::Get()->GetTimeScale() != 0)
            Time::Get()->SetTimeScale(0);
        else
            Time::Get()->SetTimeScale(1);
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

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    WinDesc desc;
    desc.appName = L"NordicLumberjack";
    desc.instance = hInstance;
    desc.handle = nullptr;
    desc.width = gWinWidth;
    desc.height = gWinHeight;
    desc.icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOPPANG));

    auto window = std::make_unique<Window>(desc);

    // 렌더러 생성 (DirectX11)
    RendererManager::Create(RenderAPI::DirectX11);
    WPARAM wParam = window->Run();

    return static_cast<int>(wParam);
}
