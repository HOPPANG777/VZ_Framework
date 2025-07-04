#pragma once

#include "Interfaces/IScene.h"
#include "Systems/Camera.h"

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
    void AddScene(const std::string& sceneName, std::shared_ptr<Scene> newScene);
    void LoadScene(const std::string& sceneName);

public:
    void SetLoadingScene(const std::string& sceneName);

    // ★ 스플래시 씬 설정
    void SetSplashScene(const std::string& sceneName);
    // ★ 게임 스타트 시, 로딩씬을 거치지 않고 바로 스플래시 → 타이틀로 가도록 처리
    void StartGame(const std::string& titleSceneName);

public:
	void SetPlayer(std::shared_ptr<GameObject> player) { this->player = player; }
	std::shared_ptr<GameObject> GetPlayer() { return player; }
	void ResetPlayer() { this->player = nullptr; }

private:
    // 안전한 씬 전환을 처리하는 내부 함수
    void ProcessSceneChange();

private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> sceneList;
    std::shared_ptr<Scene> currentScene;
    std::shared_ptr<Scene> nextScene;                         // 로드 중인 Scene


    std::shared_ptr<Scene> loadingScene;                      // 로딩 Scene
    bool isLoading = false;                              // 로딩 상태 플래그
    float loadingProgress = 0.0f;                        // 로딩 진행률

    // ★ 스플래시 씬
    std::shared_ptr<Scene> splashScene;
    // “최초 실행인가?” 를 간단히 체크하기 위한 플래그
    bool isFirstLaunch = true;

    bool sceneChangeRequested = false;

    std::shared_ptr<GameObject> player;

};

