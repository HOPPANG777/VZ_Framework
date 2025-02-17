#include "Framework.h"
#include "SceneManager.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager()
{
	// 모든 씬 정리
	for (auto& scene : sceneList)
	{
		scene.second->Destroy();
	}

	sceneList.clear();
}

void SceneManager::AddScene(const string& sceneName, shared_ptr<Scene> newScene)
{
    // 같은 이름의 Scene이 이미 있는지 확인
    if (sceneList.find(sceneName) != sceneList.end())
    {
        printf("Scene '%s' already exists.\n", sceneName.c_str());
        return;
    }
    sceneList[sceneName] = newScene;
}

void SceneManager::LoadScene(const string& sceneName)
{
	auto it = sceneList.find(sceneName);
	if (it == sceneList.end())
	{
		printf("Scene '%s' not found.\n", sceneName.c_str());
		return;
	}

	// 이미 다른 로딩이 진행 중이면 무시할 수도 있고,
	// 혹은 강제로 교체할 수도 있음(필요 로직에 맞게)
	if (isLoading)
		return;

	// 먼저 “다음에 로드될 씬”을 설정
	nextScene = it->second;

	// 만약 아직 ‘최초 실행 중’이라면, 로딩씬 없이 바로 씬 교체
	// (즉, 스플래시 → 타이틀 전환 시점에는 로딩씬을 거치지 않게 하려고)
	if (isFirstLaunch)
	{
		// 첫 전환만 로딩씬 없이 전환하고,
		// 그 다음부터는 로딩씬을 사용하도록 플래그를 꺼준다.
		isFirstLaunch = false;

		if (currentScene != nullptr)
		{
			currentScene->Destroy();
			CAMERA->Reset();
		}

		// 바로 nextScene을 현재 씬으로
		currentScene = nextScene;
		currentScene->Start();
		nextScene = nullptr;
		return;
	}

	// 그 외(최초가 아닐 때)에는 로딩씬을 거치는 로직
	if (loadingScene != nullptr)
	{
		// 현재 씬 정리
		if (currentScene != nullptr)
		{
			currentScene->Destroy();
			CAMERA->Reset();
		}

		// 로딩씬을 현재로 교체
		currentScene = nextScene;
		currentScene->Start();
		loadingScene->Start();

		// 로딩 플래그 켜고 진행률 초기화
		isLoading = true;
		loadingProgress = 0.0f;
	}
	else
	{
		// 로딩씬이 없는 경우엔 그냥 즉시 전환
		if (currentScene != nullptr)
		{
			currentScene->Destroy();
			CAMERA->Reset();
		}
		currentScene = nextScene;
		currentScene->Start();
		nextScene = nullptr;
	}
}

void SceneManager::SetLoadingScene(const string& sceneName)
{
	auto it = sceneList.find(sceneName);
	if (it != sceneList.end())
	{
		loadingScene = it->second;
	}
	else
	{
		printf("Loading Scene '%s' not found.\n", sceneName.c_str());
	}
}

void SceneManager::SetSplashScene(const string& sceneName)
{
	auto it = sceneList.find(sceneName);
	if (it != sceneList.end())
	{
		splashScene = it->second;
	}
	else
	{
		printf("Splash Scene '%s' not found.\n", sceneName.c_str());
	}
}

void SceneManager::StartGame(const string& titleSceneName)
{
	// ★ 게임을 처음 시작할 때 로딩씬 없이 “스플래시 씬”부터 보여주고,
	//   스플래시 씬이 끝나면 titleSceneName(타이틀 씬)을 LoadScene() 방식으로 전환
	//   이때 LoadScene("타이틀")을 호출하면 그때부터는 로딩씬이 쓰일 수도 있음.

	if (!splashScene)
	{
		// 스플래시 씬이 없다면, 바로 타이틀 씬으로
		LoadScene(titleSceneName);
		return;
	}

	// isFirstLaunch가 true이면 로딩 과정을 스킵하는 로직을
	// “LoadScene”에서 참고할 수 있도록 해도 되고,
	// 혹은 여기서 그냥 currentScene을 바로 스플래시로 세팅해도 됩니다.
	isFirstLaunch = true;

	// 기존 currentScene이 있다면 Destroy
	if (currentScene)
	{
		currentScene->Destroy();
		CAMERA->Reset();
	}

	// 스플래시 씬을 현재 씬으로 교체
	currentScene = splashScene;
	currentScene->Start();

	// “스플래시 -> 타이틀” 전환은 SplashScene 내부 로직(예: 일정 시간 후)에서
	// SceneManager::LoadScene(titleSceneName)을 호출해 처리한다고 가정
}

void SceneManager::Update()
{
    if (isLoading)
    {
        // 로딩씬 Update
        if (loadingScene) loadingScene->Update();

        // 로딩 진행률(예시)
        loadingProgress += 1.0f * DELTA;
        if (nextScene->IsLoaded())
        {
            // 로딩 끝났다고 가정
            // 로딩씬 -> nextScene 전환
            if (nextScene)
            {
                loadingScene->Destroy(); // 로딩씬 Destroy
                //currentScene = nextScene;
                //currentScene->Start();
                nextScene = nullptr;
            }

            isLoading = false;
        }
    }
    else
    {
        // 일반 씬 Update
        if (currentScene)
        {
            currentScene->FixedUpdate();
            currentScene->Update();
            currentScene->LateUpdate();
        }
    }
}

void SceneManager::Render()
{
	if (isLoading == true)
	{
		if (loadingScene != nullptr) loadingScene->Render();
		// 혹은 별도의 로딩 UI만 그릴 수도 있음
	}
	else
	{
		currentScene->Render();
	}
	
}

void SceneManager::PostRender()
{
	if (isLoading == true)
	{
		if (loadingScene != nullptr) loadingScene->PostRender();
	}
	else
	{
		currentScene->PostRender();
	}
}

void SceneManager::ImGUI()
{
	if (isLoading == true)
	{
		if (loadingScene != nullptr) loadingScene->ImGUI();
	}
	else
	{
		currentScene->ImGUI();
	}
}


