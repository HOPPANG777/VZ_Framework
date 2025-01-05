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
		}

		nextScene = it->second; // 로드할 Scene 저장
		isLoading = true;       // 로딩 상태 활성화
		loadingProgress = 0.0f; // 진행률 초기화
	}
	else
	{
		printf("Scene '%s' not found.\n", sceneName.c_str());
	}
}

void SceneManager::Update()
{
	if (isLoading == true)
	{
		// 로딩 진행률 업데이트
		loadingProgress += 0.1f; // 예제: 진행률 증가 (실제 작업에 맞게 수정)
		if (loadingProgress >= 1.0f)
		{
			// 로딩 완료
			currentScene = nextScene;
			currentScene->Start(); // 새로운 Scene 초기화
			nextScene = nullptr;
			isLoading = false;     // 로딩 상태 해제
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

	}
	else
	{
		currentScene->ImGUI();
	}
}


