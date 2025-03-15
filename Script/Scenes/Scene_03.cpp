#include "stdafx.h"
#include "Scene_03.h"

void Scene_03::Awake() 
{
	isLoaded = false;

	objects.push_back(make_shared<ColorFilledCircle>(CENTER + Vector3(-250, 0, 0), Vector3(100, 100, 0), 0.0f, 100, YELLOW));
	objects.push_back(make_shared<ColorCircle>(CENTER + Vector3(-100, 0, 0), Vector3(100, 100, 0), 0.0f, 100, GREEN));
	objects.push_back(make_shared<ColorLine>(CENTER + Vector3(150, 0, 0), 300.0f, 0.0f, BLUE));
	red = make_shared<ColorRect>(CENTER + Vector3(50, 0, 0), Vector3(100, 100, 0), 0.0f, RED);
	Axe = make_shared<BezierCurveAxe>(CENTER, Vector3(50, 50, 0), 0.0f);

	red->AddComponent(make_shared<MoveComponent>());

	sceneNameObject = make_shared<Object>("SceneNameText", CENTER + Vector3(-540, 335, 0), Vector3(100, 25, 0), 0.0f, TransformType::Rect);
	testTypingObject = make_shared<Object>("TestTypingText", CENTER + Vector3(0, -200, 0), Vector3(500, 25, 0), 0.0f, TransformType::Rect);
	testShakeObject = make_shared<Object>("TestShakeText", CENTER + Vector3(0, 200, 0), Vector3(200, 25, 0), 0.0f, TransformType::Rect);

	sceneNameObject->AddComponent(make_shared<Texts>());
	testTypingObject->AddComponent(make_shared<TypingText>());
	testShakeObject->AddComponent(make_shared<ShakingText>());

	UINT spacing = 28;
	UINT width = (WIN_DEFAULT_WIDTH * 2) / spacing;
	UINT height = (WIN_DEFAULT_HEIGHT * 2) / spacing;

	tmap = make_unique<TileMap>(spacing, width, height);

	string mapFilePath = "_Data/_TileData/TestMap.data";

	tmap->Load(mapFilePath);

	isLoaded = true;
}

void Scene_03::Start()
{
	SOUNDS->AddSound("BGM", SoundPath + L"bgm.wav", true);

	SOUNDS->Play("BGM", 0.2f);

	CAMERA->SetTarget(red.get());
}

void Scene_03::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();

	if (red != nullptr) red.reset();
	if (Axe != nullptr) Axe.reset();

	if (sceneNameObject != nullptr) sceneNameObject.reset();
	if (testTypingObject != nullptr) testTypingObject.reset();
	if (testShakeObject != nullptr) testShakeObject.reset();
	
	if (tmap != nullptr) tmap.reset();

	SOUNDS->RemoveSound("BGM");
}

void Scene_03::FixedUpdate() {}

void Scene_03::Update()
{
	auto rect = dynamic_pointer_cast<ColorRect>(red);

	if (INPUT->KeyDown('Q'))
	{
		if (rect->GetColor() == BLUE) rect->SetColor(RED);
		else if (rect->GetColor() == RED) rect->SetColor(BLUE);
	}

	if (INPUT->KeyDown(VK_F1))
	{
		SCENE->LoadScene("ClockScene");
	}

	if (tmap != nullptr) tmap->Update();

	if (testTypingObject != nullptr) testTypingObject->Update();

	for (const auto& obj : objects) if (obj != nullptr) obj->Update();
	if (red != nullptr) red->Update();
	if (Axe != nullptr) Axe->Update();
}

void Scene_03::LateUpdate() {}

void Scene_03::Render()
{
	if (tmap != nullptr) tmap->Render();
	for (const auto& obj : objects) if (obj != nullptr) obj->Render();
	if (Axe != nullptr) Axe->Render();
	if (red != nullptr) red->Render();
}

void Scene_03::PostRender() 
{
	sceneNameObject->GetComponent<Texts>("Texts")->RenderText(L"GeometryScene", sceneNameObject->GetRect(), 25.0f);
	testTypingObject->GetComponent<TypingText>("TypingText")->RenderText(L"안녕하세요 조승영입니다 이건 테스트입니다.", testTypingObject->GetRect(), 25.0f);
	testShakeObject->GetComponent<ShakingText>("ShakingText")->RenderText(L"그라라라라라라라라.", testShakeObject->GetRect(), 25.0f);
}

void Scene_03::ImGUI() {}

//objects.back()->AddComponent(make_shared<BoxCollider>("Rect", Vector3(0, 0, 0), Vector3(150, 150, 0), RED));
//objects[1]->AddComponent(make_shared<BoxCollider>("Circle", Vector3(0, 0, 0), Vector3(100, 100, 0)));

//COLLISION->AddCollider(objects[3]->GetComponent<BoxCollider>("Rect"));
//COLLISION->AddCollider(objects[1]->GetComponent<BoxCollider>("Circle"));