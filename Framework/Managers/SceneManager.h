#pragma once

#include "Interfaces/IScene.h"

class SceneManager
{
    DECLARE_SINGLETON(SceneManager);
public:
    void AddScene(const string& sceneName, shared_ptr<Scene> newScene);
    void LoadScene(const string& sceneName);

    void Update();

    void Render();
    void PostRender();

    void ImGUI();

private:
    unordered_map<string, shared_ptr<Scene>> sceneList;
    shared_ptr<Scene> currentScene;
    shared_ptr<Scene> nextScene;                         // 로드 중인 Scene
    bool isLoading = false;                              // 로딩 상태 플래그
    float loadingProgress = 0.0f;                        // 로딩 진행률

};

