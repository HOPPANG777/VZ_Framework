#include "stdafx.h"
#include "Scene_06.h"

void Scene_06::Awake()
{
	isLoaded = false;

	objects.push_back(make_shared<ColorLine>(CENTER + Vector3(-640, -100, 0), 5000, 0.0f, RED));

	testPlayer = make_shared<ColorRect>(CENTER + Vector3(-640, -100, 0), Vector3(100, 100, 0), 0.0f, RED);

	objects[0]->AddComponent(make_shared<BoxCollider>("Floor01", Vector3(0, 0, 0), Vector3(5000, 1, 0), GREEN));
	testPlayer->AddComponent(make_shared<BoxCollider>("testPlayer", Vector3(0, 0, 0), Vector3(100, 100, 0), BLUE));
    testPlayer->AddComponent(make_shared<MoveComponent>());

	auto object01Collider = objects[0]->GetComponent<BoxCollider>("Floor01");
	auto testPlayerCollider = testPlayer->GetComponent<BoxCollider>("testPlayer");

	if (object01Collider) COLLISION->AddCollider(object01Collider);
	if (testPlayerCollider) COLLISION->AddCollider(testPlayerCollider);

	isLoaded = true;
}

void Scene_06::Start()
{

}

void Scene_06::Destroy()
{
	for (auto& obj : objects) if (obj != nullptr) obj.reset();
	if (testPlayer != nullptr) testPlayer.reset();
}

void Scene_06::FixedUpdate() {}

void Scene_06::Update()
{
    //if (tmap != nullptr) tmap->Update();

    for (const auto& obj : objects) if (obj != nullptr) obj->Update();

    if (testPlayer)
    {
        // 중력 상수 (예: 980 픽셀/s², DELTA는 초 단위)
        const float gravityAcc = 50000.0f;

        // testPlayer에 중력 적용 (아래로 가속)
        testPlayerVelocity.y -= (gravityAcc * 2.0f) * DELTA;

        // testPlayer를 이동 (이동은 Move() 함수가 내부에서 position을 더하는 방식)
        testPlayer->Move(testPlayerVelocity * DELTA);
;

        // floor와 testPlayer의 충돌 검사
        auto floorCollider = objects[0]->GetComponent<BoxCollider>("Floor01");
        auto playerCollider = testPlayer->GetComponent<BoxCollider>("testPlayer");

        if (floorCollider && playerCollider)
        {
            // BoxCollider::IsColliding()를 통해 충돌 여부 판단
            if (floorCollider->IsColliding(playerCollider.get()))
            {
                // floor의 top (world 좌표 기준)
                float floorTop = floorCollider->GetCenter().y + (floorCollider->GetSize().y * 0.5f);
                // testPlayer의 bottom (world 좌표 기준)
                float playerBottom = playerCollider->GetCenter().y - (playerCollider->GetSize().y * 0.5f);

                // 만약 testPlayer가 floor 안으로 침투했다면, 보정값 계산
                float correction = floorTop - playerBottom;
                if (correction > 0)
                {
                    // testPlayer의 위치를 보정하여 floor 위에 서도록 함
                    testPlayer->GetComponent<Transform>("Transform")->SetPosition(testPlayer->GetPosition() + Vector3(0, correction, 0));
                    // 충돌 시 수직 속도를 0으로 초기화
                    testPlayerVelocity.y = 0;
                }
            }
        }

        if (testPlayer != nullptr) testPlayer->Update();
    }
}

void Scene_06::LateUpdate() {}

void Scene_06::Render()
{
	//if (tmap != nullptr) tmap->Render();

	for (const auto& obj : objects) if (obj != nullptr) obj->Render();

	if (testPlayer != nullptr) testPlayer->Render();
}

void Scene_06::PostRender()
{

}

void Scene_06::ImGUI()
{

}


