#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake()
{
}

void Scene_03::Start()
{
}

void Scene_03::Destroy()
{
}

void Scene_03::FixedUpdate()
{
}

void Scene_03::Update()
{
	if (KEYBOARD->Press('E'))
	{
		SCENE->LoadScene("ClockScene");
	}
}

void Scene_03::LateUpdate()
{
}

void Scene_03::Render()
{
}

void Scene_03::PostRender()
{
}

void Scene_03::ImGUI()
{
}
