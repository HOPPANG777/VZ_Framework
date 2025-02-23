#pragma once

#include "Interfaces/IScene.h"

class SceneManager
{
    DECLARE_SINGLETON(SceneManager);    
public:
    void Start();

    void Update();

    void Render();
    void PostRender();

    void ImGUI();

public:
    void AddScene(const string& sceneName, shared_ptr<Scene> newScene);
    void LoadScene(const string& sceneName);

public:
    void SetLoadingScene(const string& sceneName);

    // ★ 스플래시 씬 설정
    void SetSplashScene(const string& sceneName);
    // ★ 게임 스타트 시, 로딩씬을 거치지 않고 바로 스플래시 → 타이틀로 가도록 처리
    void StartGame(const string& titleSceneName);

private:
    // 안전한 씬 전환을 처리하는 내부 함수
    void ProcessSceneChange();

private:
    unordered_map<string, shared_ptr<Scene>> sceneList;
    shared_ptr<Scene> currentScene;
    shared_ptr<Scene> nextScene;                         // 로드 중인 Scene


    shared_ptr<Scene> loadingScene;                      // 로딩 Scene
    bool isLoading = false;                              // 로딩 상태 플래그
    float loadingProgress = 0.0f;                        // 로딩 진행률

    // ★ 스플래시 씬
    shared_ptr<Scene> splashScene;
    // “최초 실행인가?” 를 간단히 체크하기 위한 플래그
    bool isFirstLaunch = true;

    bool sceneChangeRequested = false;

};

