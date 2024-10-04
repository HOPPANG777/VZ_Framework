#include "Framework.h"
#include "SceneManager.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager()
{
	for (auto& scene : sceneList)
	{
		scene->Destroy();
	}
}

void SceneManager::AddScene(shared_ptr<Scene> newScene)
{
	sceneList.push_back(newScene);
}

void SceneManager::LoadScene(int sceneIndex)
{
	if (sceneIndex >= 0 && sceneIndex < sceneList.size())
	{
		if (currentScene != nullptr)
		{
			currentScene->Destroy();  // 이전 Scene 정리
		}

		currentScene = sceneList[sceneIndex];
		currentScene->Start();        // 새로운 Scene 초기화
	}
}

void SceneManager::Update()
{
	if (currentScene != nullptr)
	{
		currentScene->FixedUpdate();
		currentScene->Update();
		currentScene->LateUpdate();
	}
}

void SceneManager::Render()
{
	if (currentScene != nullptr)
	{
		currentScene->Render();
	}
}

void SceneManager::PostRender()
{
	if (currentScene != nullptr)
	{
		currentScene->PostRender();
	}
}

void SceneManager::ImGUI()
{
	if (currentScene != nullptr)
	{
		currentScene->ImGUI();
	}
}


