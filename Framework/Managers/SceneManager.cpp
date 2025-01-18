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
	if (it != sceneList.end())
	{
		if (currentScene != nullptr)
		{
			currentScene->Destroy(); // 기존 Scene 정리

			CAMERA->Reset();
		}

		nextScene = it->second; // 로드할 Scene 저장

		// 3) 로딩씬을 currentScene으로 교체
		// (SetLoadingScene("Scene_00")를 통해 미리 loadingScene 설정해둔 상태)
		if (loadingScene != nullptr)
		{
			currentScene = loadingScene;
			currentScene->Start(); // 로딩씬의 Start()
		}

		isLoading = true;       // 로딩 상태 활성화
		loadingProgress = 0.0f; // 진행률 초기화
	}
	else
	{
		printf("Scene '%s' not found.\n", sceneName.c_str());
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

void SceneManager::Update()
{
	if (isLoading == true)
	{
		// 로딩씬도 씬이므로, 필요하다면 FixedUpdate/LateUpdate 순서를 어떻게 할지도 고민
		// 여기서는 간단하게 Update()만 호출한다고 가정
		if (loadingScene != nullptr)
		{
			loadingScene->Update();
		}

		// 로딩 진행률 업데이트 (실제 로직에 맞게 변경)
		loadingProgress += 1.0f * DELTA;

		// 예: "로딩이 끝났다"라고 판단하는 조건
		if (loadingProgress >= 1.5f)
		{
			// 로딩씬 끝났으니, 필요하면 로딩씬을 Destroy()하거나, 
			// 계속 재사용하려면 Destroy하지 않아도 됨
			// loadingScene->Destroy();
			// loadingScene = nullptr;

			// 다음 씬으로 전환
			currentScene = nextScene;
			currentScene->Start();
			nextScene = nullptr;

			isLoading = false;
		}
	}
	else
	{
		currentScene->FixedUpdate();
		currentScene->Update();
		currentScene->LateUpdate();
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


