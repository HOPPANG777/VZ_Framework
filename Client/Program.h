#pragma once

#include "Systems/Graphics.h"
#include "Systems/Input.h"
#include "Systems/Time.h"
#include "Systems/Window.h"
#include "Managers/SceneManager.h"
#include "Managers/CollisionManager.h"

class Program
{
public:
	Program();
	~Program();

public:
	void Awake();
	void Start();

	void Update();

	void Render();
	void PostRender();

	void ImGUI();

private:
	//vector<shared_ptr<Scene>> sceneList;
	//shared_ptr<Scene> currentScene;

	bool isFullScreen = false;
};
