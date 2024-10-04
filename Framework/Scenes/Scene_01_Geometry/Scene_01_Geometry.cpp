#include "Framework.h"
#include "Scene_01_Geometry.h"

void Scene_01::Awake()
{

}

void Scene_01::Start()
{
	rect = make_unique<ColorRect>(CENTER, Vector2(100, 100), 0.0f, RED);
	line = make_unique<ColorLine>(CENTER, Vector2(1000, 1), 0.0f, RED);

	rect->AddComponent(make_shared<MoveComponent>());
}

void Scene_01::Destroy()
{
	rect.reset();
	line.reset();
}

void Scene_01::FixedUpdate()
{

}

void Scene_01::Update()
{
	if (INPUT->Down('Q'))
	{
		if (rect->GetColor() == BLUE) rect->SetColor(RED);
		else if (rect->GetColor() == RED) rect->SetColor(BLUE);
	}

	rect->Update();
	line->Update();
}

void Scene_01::LateUpdate()
{

}

void Scene_01::Render()
{
	rect->Render();
	line->Render();
}

void Scene_01::PostRender()
{

}

void Scene_01::ImGUI()
{

}
