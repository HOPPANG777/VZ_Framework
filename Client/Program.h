#pragma once

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
	
};
