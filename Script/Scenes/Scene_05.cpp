#include "stdafx.h"
#include "Scene_05.h"

void Scene_05::Awake() {}

void Scene_05::Start()
{
    timer = 0.0f;
    isDone = false;
    // 스플래시 씬 초기화
    
    objects.push_back(make_shared<ColorRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, BLACK));
    objects.push_back(make_shared<TextureRect>(CENTER, Vector3(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT, 0), 0.0f, TexturePath + L"SplashLogo.png"));
    
    // 예) 로고 이미지 로드, 사운드 재생 등
}

void Scene_05::Destroy()
{
    for (auto& obj : objects) if (obj != nullptr) obj.reset();
}

void Scene_05::FixedUpdate() {}

void Scene_05::Update()
{
    if (!isDone)
    {
        timer += DELTA;
        // 예: 3초 후 자동으로 타이틀 씬으로
        if (timer >= 3.0f)
        {
            isDone = true;
            // 스플래시 종료 후 타이틀 씬으로 이동
            SCENE->LoadScene("TitleScene");
            //SCENE->LoadScene("GeometryScene");
        }
    }
    // 그 외 스플래시 애니메이션 업데이트

    //objects[0]->

    for (const auto& obj : objects) if (obj != nullptr) obj->Update();
}

void Scene_05::LateUpdate() {}

void Scene_05::Render()
{
    for (const auto& obj : objects) if (obj != nullptr) obj->Render();
}

void Scene_05::PostRender()
{

}

void Scene_05::ImGUI() {}
