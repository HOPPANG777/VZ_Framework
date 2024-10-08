#pragma once

#include "Interfaces/IScene.h"

class SceneManager
{
    DECLARE_SINGLETON(SceneManager);
public:
    void AddScene(shared_ptr<Scene> newScene);
    void LoadScene(int sceneIndex);

    void Update();

    void Render();
    void PostRender();

    void ImGUI();

private:
    vector<shared_ptr<Scene>> sceneList;
    shared_ptr<Scene> currentScene;
};

