#pragma once

#include "Systems/Graphics.h"
#include "Systems/Window.h"
#include "Utilities/Gui.h"
#include "Managers/SceneManager.h"

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
