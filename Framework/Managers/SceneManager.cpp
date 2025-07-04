#include "SceneManager.h"
#include <cstdio>

SceneManager::SceneManager() {}

// SceneManager 소멸자: 모든 씬을 Destroy() 호출 후 sceneList를 clear합니다.
SceneManager::~SceneManager()
{
	// std::unordered_map<std::string, std::shared_ptr<Scene>>::iterator 사용
	for (auto& scene : sceneList)
	{
		scene.second->Destroy();
	}
	sceneList.clear();
}

// 씬을 추가합니다.
void SceneManager::AddScene(const std::string& sceneName, std::shared_ptr<Scene> newScene)
{
	// 같은 이름의 씬이 이미 존재하는지 확인
	if (sceneList.find(sceneName) != sceneList.end())
	{
		printf("Scene '%s' already exists.\n", sceneName.c_str());
		return;
	}
	// 씬 추가
	sceneList[sceneName] = newScene;
}

// 씬을 로드하는 함수: sceneName에 해당하는 씬을 nextScene으로 지정하고, 첫 실행인 경우 바로 전환합니다.
void SceneManager::LoadScene(const std::string& sceneName)
{
	// 명시적 iterator 타입 사용
	const auto& it = sceneList.find(sceneName);
	if (it == sceneList.end())
	{
		printf("Scene '%s' not found.\n", sceneName.c_str());
		return;
	}
	// 이미 로딩 중이면 무시
	if (isLoading)
	{
		return;
	}

	nextScene = it->second;
	sceneChangeRequested = true;

	// 만약 최초 실행이면 바로 전환 (deferred 처리 없이)
	if (isFirstLaunch)
	{
		isFirstLaunch = false;
		if (currentScene)
		{
			currentScene->Destroy();
			Camera::Get()->Reset();
		}
		currentScene = nextScene;
		currentScene->Awake();
		currentScene->Start();
		nextScene = nullptr;
		sceneChangeRequested = false;
	}
}

// Loading Scene을 설정하는 함수
void SceneManager::SetLoadingScene(const std::string& sceneName)
{
	const auto& it = sceneList.find(sceneName);
	if (it != sceneList.end())
	{
		loadingScene = it->second;
	}
	else
	{
		printf("Loading Scene '%s' not found.\n", sceneName.c_str());
	}
}

// Splash Scene을 설정하는 함수
void SceneManager::SetSplashScene(const std::string& sceneName)
{
	const auto& it = sceneList.find(sceneName);
	if (it != sceneList.end())
	{
		splashScene = it->second;
	}
	else
	{
		printf("Splash Scene '%s' not found.\n", sceneName.c_str());
	}
}

void SceneManager::StartGame(const std::string& titleSceneName)
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
		Camera::Get()->Reset();
	}

	// 스플래시 씬을 현재 씬으로 교체
	currentScene = splashScene;
	currentScene->Awake();

	// “스플래시 -> 타이틀” 전환은 SplashScene 내부 로직(예: 일정 시간 후)에서
	// SceneManager::LoadScene(titleSceneName)을 호출해 처리한다고 가정
}

void SceneManager::ProcessSceneChange()
{
	if (sceneChangeRequested && nextScene)
	{
		// 로딩씬이 존재하면 로딩씬을 거치는 로직
		if (loadingScene != nullptr)
		{
			// 현재 씬 정리
			if (currentScene != nullptr)
			{
				currentScene->Destroy();
				Camera::Get()->Reset();
			}

			// nextScene을 현재 씬으로 설정하고, 로딩씬을 시작합니다.
			currentScene = nextScene;
			currentScene->Awake();
			loadingScene->Start();

			// 로딩 플래그를 활성화하고 진행률 초기화
			isLoading = true;
			loadingProgress = 0.0f;
		}
		else
		{
			// 로딩씬이 없는 경우엔 즉시 전환
			if (currentScene != nullptr)
			{
				currentScene->Destroy();
				Camera::Get()->Reset();
			}
			currentScene = nextScene;
			currentScene->Awake();
			nextScene = nullptr;
		}
		sceneChangeRequested = false;
	}
}

void SceneManager::Start()
{
	if (isLoading == true)
	{
		if (loadingScene != nullptr) loadingScene->Start();
		// 혹은 별도의 로딩 UI만 그릴 수도 있음
	}
	else
	{
		currentScene->Start();
	}
}

void SceneManager::Update()
{
    if (isLoading)
    {
        // 로딩씬 Update
        if (loadingScene) loadingScene->Update();

        // 로딩 진행률(예시)
        if (nextScene->IsLoaded())
        {
            // 로딩 끝났다고 가정
            // 로딩씬 -> nextScene 전환
            if (nextScene)
            {
                loadingScene->Destroy(); // 로딩씬 Destroy
                //currentScene = nextScene;
                currentScene->Start();
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

	// deferred 씬 전환 처리
	ProcessSceneChange();
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


